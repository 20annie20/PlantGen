#include "Tree.h"
#include "LSystem.h"
#include "ParametricSystem.h"
#include "Components/InstancedStaticMeshComponent.h"

Branch::Branch() {
	state = ACTIVE;
	nodes.Add({ FVector(0.0), FVector(0.0, 0.0, 1.0), 0, TArray<bool>()});
	parent = NULL;
	start_width = 50.0; // TODO - get from scaling, default 50 cm wide
	end_width = 10.0;
	parent_node_idx = 0;
	parent_idx = 0;
	this->parent_idx = parent_idx;
}

Branch::Branch(Branch* parent, int parent_idx) {
	state = ACTIVE;
	nodes.Add(Node(parent->nodes.Last()));
	parent_node_idx = parent->nodes.Num() - 1; // get index of parent's index for which this branch grows
	this->parent_idx = parent_idx;
	this->parent = parent;
	this->start_width = parent->end_width * 2.0;
	this->end_width = start_width / (start_width * 0.1);
}

void Branch::AddNode(const int numBuds) {
	// positions will be recalculated at the end of generation
	nodes.Add({ FVector(0.0), FVector(0.0, 0.0, 1.0), 0, TArray<bool>() });
	for (int i = 0; i < numBuds; i++) {
		nodes.Last().buds.Add(true);
	}
}

// Sets default values
ATree::ATree()
{
	//PrimaryActorTick.bCanEverTick = true;
	GenerateTree();
}

void ATree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
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

	species = BIRCH;
	int tree_age = 20;
	ParametricSystem simulator = ParametricSystem(species, tree_age);

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
			spline->AddSplineLocalPoint(node.coordinates);
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

				auto scale_start = spline.Value->nodes[i].age * 0.5 / longest_edge;
				auto scale_end = spline.Value->nodes[i + 1].age * 0.5 / longest_edge;
				smc->SetStaticMesh(TreeStaticMesh);
				smc->SetStartAndEnd(start_location, start_tangent, end_location, end_tangent);
				smc->SetStartScale(TVector2(scale_start));
				smc->SetEndScale(TVector2(scale_end));
			}
		}
	}

}

void ATree::RefreshTree()
{	
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
			spline.Key->GetLocalLocationAndTangentAtSplinePoint(i + 1, end_location, end_tangent);

			FName name = FName(spline.Key->GetFName().ToString() + "_" + FString::FormatAsNumber(i) + "_smc");
			USplineMeshComponent* smc = (USplineMeshComponent*)GetDefaultSubobjectByName(name);

			if (smc != NULL) {
				if (TreeStaticMesh != NULL) {

					auto scale_start = spline.Value->nodes[i].age * 0.5 / longest_edge;
					auto scale_end = spline.Value->nodes[i + 1].age * 0.5 / longest_edge;
					smc->SetStaticMesh(TreeStaticMesh);
					smc->SetStartAndEnd(start_location, start_tangent, end_location, end_tangent);
					smc->SetStartScale(TVector2(scale_start));
					smc->SetEndScale(TVector2(scale_end));
				}
			}
		}
	}
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

