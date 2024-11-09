#include "Components/InstancedStaticMeshComponent.h"
#include "Tree.h"
#include "LSystem.h"


// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//InstancedTreeComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedTreeComponent"));
	//InstancedTreeComponent->SetupAttachment(GetRootComponent());

	//InstancedLeafComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedLeafComponent"));
	//InstancedLeafComponent->SetupAttachment(GetRootComponent());

	GenerateTree();
}

void ATree::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*if (!InstancedTreeComponent->GetStaticMesh())
	{
		InstancedTreeComponent->SetStaticMesh(TreeStaticMesh);
		InstancedTreeComponent->SetMaterial(0, UMaterialInstanceDynamic::Create(TreeMaterial, this));

	}*/
}

void ATree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshTree();
}


void ATree::SetAngle(float NewAngle)
{
	Angle = NewAngle;
	RefreshTree();
}

void ATree::SetAxiom(FString NewAxiom)
{
	Axiom = NewAxiom;
	RefreshTree();
}

void ATree::SetRadius(float NewRadius)
{
	Radius = NewRadius;
	RefreshTree();
}

void ATree::SetIterations(int NewIterations)
{
	if (NewIterations != 0){
		Iterations = NewIterations;
	}
	else {
		Iterations = 1;
	}
	
	RefreshTree();
}

void ATree::SetLength(float NewLength)
{
	Length = NewLength;
	RefreshTree();
}

void ATree::SetBranchRadiusReduction(float NewBranchRadiusReduction)
{
	BranchRadiusReduction = NewBranchRadiusReduction;
	RefreshTree();
}

void ATree::SetMinBranchRadius(float NewMinBranchRadius)
{
	MinBranchRadius = NewMinBranchRadius;
	RefreshTree();
}

void ATree::SetBranchLengthRelativeToParent(float NewBranchLengthRelativeToParent)
{
	BranchLengthRelativeToParent = NewBranchLengthRelativeToParent;
	RefreshTree();
}

void ATree::SetLeafSize(float NewLeafSize)
{
	LeafSize = NewLeafSize;
	RefreshTree();
}


void ATree::GenerateTree()
{
	UE_LOG(LogTemp, Warning, TEXT("Generating Splines."));

	FString initial_tree = Axiom;
	float scale = 1.0f / Iterations;

	TMap<TCHAR, TArray<FString>> rules = {
		{ 'X', {"F[+X][-X]FX"}},
		{ 'F', {"FF"}}
	};

	LSystem simulator = LSystem(initial_tree, rules, Iterations);

	FString word = simulator.produce_word();

	TArray<TArray<FVector>> branches = simulator.join_branches_for_splines(Angle, scale);

	int it = 0;
	for (auto& branch : branches)
	{
		FName name = FName(FString::FormatAsNumber(it) + "_spline");
		USplineComponent* spline = (USplineComponent*)GetDefaultSubobjectByName(name);
		if (spline == NULL){
			spline = CreateDefaultSubobject<USplineComponent>(name);
			spline->SetupAttachment(GetRootComponent());
			splines.Add(spline);
		}
		spline->ClearSplinePoints();

		it++;

		for (int i = 0; i < branch.Num(); i++) {
			spline->AddSplineWorldPoint(branch[i]);
		}
	}
	PopulateMesh();
}

void ATree::PopulateMesh() {
	
	for (auto spline : splines) {
		for (int i = 0; i < spline->GetNumberOfSplinePoints() - 1; i++) {
			FVector start_location, start_tangent, end_location, end_tangent;

			spline->GetLocalLocationAndTangentAtSplinePoint(i, start_location, start_tangent);
			spline->GetLocalLocationAndTangentAtSplinePoint(i+1, end_location, end_tangent);

			FName name = FName(spline->GetFName().ToString() + "_" + FString::FormatAsNumber(i) + "_smc");
			USplineMeshComponent* smc = (USplineMeshComponent*)GetDefaultSubobjectByName(name);

			if (smc == NULL) {
				smc = CreateDefaultSubobject<USplineMeshComponent>(name);
				smc->AttachToComponent(spline, FAttachmentTransformRules::KeepRelativeTransform);
				smc->SetForwardAxis(ESplineMeshAxis::Type::Z);
			}
			if (TreeStaticMesh != NULL) {
				smc->SetStaticMesh(TreeStaticMesh);
				smc->SetStartAndEnd(start_location, start_tangent, end_location, end_tangent);
			}
			
		}
	}

}

void ATree::RefreshTree()
{
	//InstancedTreeComponent->ClearInstances();
	//InstancedLeafComponent->ClearInstances();
	
	GenerateTree();
}

/*
FInstanceData ATree::CreateInstanceData(const FTransform& Transform, TEnumAsByte<EInstanceType> InstanceType)
{
	FInstanceData InstanceData;
	InstanceData.Transform = Transform;
	InstanceData.InstanceType = InstanceType;
	return InstanceData;
}
*/

