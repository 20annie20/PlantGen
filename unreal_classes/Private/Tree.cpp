#include "Tree.h"
#include "LSystem.h"
#include "ParametricSystem.h"
#include "Components/InstancedStaticMeshComponent.h"

Branch::Branch() {
	state = ACTIVE;
	nodes_count = 1;
	nodes.Add(FVector(0.0));
	parent = NULL;
	is_terminal = true;
	start_width = 50.0; // TODO - get from scaling, default 50 cm wide
	end_width = 0.0;
}

Branch::Branch(Branch* parent) {
	state = ACTIVE;
	nodes_count = 1;
	nodes.Add(FVector(parent->nodes.Last()));
	this->parent = parent;
	this->parent->is_terminal = false;
	this->is_terminal = true;
	this->start_width = this->parent->end_width;
	end_width = 0.0;
}

void Branch::AddNode() {
	nodes_count++;
}

// Sets default values
ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = true;
	GenerateTree();
}

void ATree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshTree();
}

void ATree::SetSpecies(Species NewSpecies)
{
	species = NewSpecies;
	RefreshTree();
}

/*
void ATree::SetAngle(float NewAngle)
{
	Angle = NewAngle;
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
} */


void ATree::GenerateTree()
{
	UE_LOG(LogTemp, Warning, TEXT("Generating Splines."));

	/*FString initial_tree = "X";

	float scale = 1.0f / Iterations;

	TMap<TCHAR, TArray<FString>> rules = {
		{ 'X', {"F[+X][-X]FX"}},
		{ 'F', {"FF"}}
	};

	LSystem simulator = LSystem(initial_tree, rules, Iterations);

	FString word = simulator.produce_word(); */

	ParametricSystem simulator = ParametricSystem(species);

	grown_branches = simulator.GrowBranches();
	splines.Empty();

	int it = 0;
	for (auto& branch : grown_branches)
	{
		FName name = FName(FString::FormatAsNumber(it) + "_spline");
		USplineComponent* spline = (USplineComponent*)GetDefaultSubobjectByName(name);
		if (spline == NULL){
			spline = CreateDefaultSubobject<USplineComponent>(name);
			spline->SetupAttachment(GetRootComponent());
			
		}
		splines.Add({ spline, &branch });
		spline->ClearSplinePoints();
		it++;

		for (auto& node : branch.nodes) {
			spline->AddSplineLocalPoint(node);
		}
	}
	PopulateMesh();
}

void ATree::PopulateMesh() {

	auto longest_edge = 0.0;

	if (TreeStaticMesh != NULL) {
		TreeStaticMesh->CalculateExtendedBounds();
		FBox box = TreeStaticMesh->GetBoundingBox();
		FVector size = box.GetSize();
		longest_edge = FMath::Max(size.X, size.Y);
	}
	
	for (auto spline : splines) {
		for (int i = 0; i < spline.Key->GetNumberOfSplinePoints() - 1; i++) {
			FVector start_location, start_tangent, end_location, end_tangent;

			spline.Key->GetLocalLocationAndTangentAtSplinePoint(i, start_location, start_tangent);
			spline.Key->GetLocalLocationAndTangentAtSplinePoint(i+1, end_location, end_tangent);

			FName name = FName(spline.Key->GetFName().ToString() + "_" + FString::FormatAsNumber(i) + "_smc");
			USplineMeshComponent* smc = (USplineMeshComponent*)GetDefaultSubobjectByName(name);

			if (smc == NULL) {
				smc = CreateDefaultSubobject<USplineMeshComponent>(name);
				smc->AttachToComponent(spline.Key, FAttachmentTransformRules::KeepRelativeTransform);
				smc->SetForwardAxis(ESplineMeshAxis::Type::Z);
			}
			if (TreeStaticMesh != NULL) {
				smc->SetStaticMesh(TreeStaticMesh);
				smc->SetStartAndEnd(start_location, start_tangent, end_location, end_tangent);
				
				auto start_scale = spline.Value->start_width / longest_edge;
				auto end_scale = spline.Value->end_width / longest_edge;
				
				smc->SetStartScale(TVector2(start_scale));
				smc->SetEndScale(TVector2(end_scale));
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

