// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

static const FName TAG_Spline("Spline");
static const FName TAG_Branch("Branch");
static const FName TAG_Leaves("Leaves");

int State::CalcHeight(int startIdx) const {
	int height = (branches[startIdx].rootTreeDepth + branches[startIdx].nodes.Num());
	int childIdx = startIdx + 1;

	for (int i = 0; i < branches[startIdx].numChildBranches; ++i) {
		height = FMath::Max(height, CalcHeight(childIdx));
		childIdx += (branches[childIdx].numChildBranches + 1);
	}

	return height;
}

// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Branch thickness scales.
	RootBranchThickness = 1.0f;
	LastBranchThickness = 0.05f;
	BranchLevelThicknessFactor = 2.0f;

	simulator = ParametricSimulator(species_id, GetWorld());

	GenerateTree();
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATree::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, age))
	{
		for (int i = states.Num() - 1; i < age; i++) {
			states.Add(simulator.GrowTree(states[i], i, false));
		}
		RefreshMesh(states[age]);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, species_id))
	{
		auto world = GetWorld();
		simulator = ParametricSimulator(species_id, world);
		ClearTree();
		GenerateTree();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, TreeStaticMesh)) {
		AssignTreeMesh();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, TreeMaterial)) {
		AssignTreeMaterial();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, LeafStaticMesh)) {
		AssignLeafMesh();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, LeafMaterial)) {
		AssignLeafMaterial();
	}
	else if ((PropertyName == GET_MEMBER_NAME_CHECKED(ATree, RootBranchThickness)) ||
			 (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, LastBranchThickness)) ||
			 (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, BranchLevelThicknessFactor)) ||
			 (PropertyName == GET_MEMBER_NAME_CHECKED(ATree, LeafScale))) {
		if(states.Num())
			RefreshMesh(states.Last());
	}
	Super::PostEditChangeProperty(e);
}


void ATree::GenerateTree()
{
	UE_LOG(LogTemp, Warning, TEXT("Generating Tree."));
	
	auto initial_branch = Branch();
	TArray<Branch> branches = { initial_branch };
	State state = { branches };
	states.Add(state);
}

void ATree::RefreshMesh(const State& state) {
	FRandomStream random;

	// TODO do not create new component if such component already exists - check smh
	// TODO clear splines completely in the clearTree() method
	for (int32 idx = 0; idx < state.branches.Num(); idx++) {

		// Calculate max number of nodes in the current branch
		const int treeHeight = state.CalcHeight(idx);

		auto branch_name = FName(FString::FormatAsNumber(idx) + "_spline");
		USplineComponent* SplineComponent = (USplineComponent*)GetDefaultSubobjectByName(branch_name);
		if (SplineComponent == NULL) {
			SplineComponent = NewObject<USplineComponent>(this, branch_name);
			SplineComponent->SetMobility(EComponentMobility::Static);
			SplineComponent->RegisterComponent();
			SplineComponent->ComponentTags.Add(TAG_Spline);
		}

		// set the spline to default state, but with no points
		SplineComponent->ResetToDefault();
		SplineComponent->ClearSplinePoints();

		// Iterate over our input points and add spline points to the spline component
		for (int n_idx = 0; n_idx < state.branches[idx].nodes.Num() - 1; n_idx++)
		{
			auto point = state.branches[idx].nodes[n_idx].coordinates;
			int32 AddIdx = SplineComponent->GetNumberOfSplinePoints();
			SplineComponent->AddSplinePointAtIndex(point, AddIdx, ESplineCoordinateSpace::Local);

			FVector startLocation, startTangent;
			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(n_idx, startLocation, startTangent);

			FVector endLocation, endTangent;
			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(n_idx + 1, endLocation, endTangent);

			// assign mesh to each spline point
			FName name = FName(SplineComponent->GetFName().ToString() + "_" + FString::FormatAsNumber(n_idx) + "_smc");
			USplineMeshComponent* smc = (USplineMeshComponent*)GetDefaultSubobjectByName(name);
			
			if (smc == NULL) {
				smc = NewObject<USplineMeshComponent>(this, name);
				smc->SetMobility(EComponentMobility::Static);
				smc->SetForwardAxis(ESplineMeshAxis::Type::Z);
				smc->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
				smc->ComponentTags.Add(TAG_Branch);

				// TODO WA the end_tangent problem && clean the meshes from the smcs on the state restore 
				smc->SetStartAndEnd(
					state.branches[idx].nodes[n_idx].coordinates, startTangent,
					state.branches[idx].nodes[n_idx+1].coordinates, endTangent);

				if (TreeStaticMesh) {
					smc->SetStaticMesh(TreeStaticMesh);
				}
				if (TreeMaterial) {
					smc->SetMaterial(0, TreeMaterial);
				}
			}

			UpdateBranchThickness(smc, state.branches[idx], n_idx, treeHeight);
		}

		// Create a plane with leaves at the end of each branch.
		if (state.branches[idx].state == BranchState::Active) {
			Node node = state.branches[idx].nodes.Last();
			FName name = FName(SplineComponent->GetFName().ToString() + "_leaves_mc");
			UStaticMeshComponent* mc = (UStaticMeshComponent*)GetDefaultSubobjectByName(name);

			if (mc == NULL) {
				mc = NewObject<UStaticMeshComponent>(this, name);
				mc->SetMobility(EComponentMobility::Static);
				mc->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
				mc->ComponentTags.Add(TAG_Leaves);

				FVector location, tangent;
				SplineComponent->GetLocalLocationAndTangentAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, location, tangent);
				mc->SetRelativeLocation(location);

				FRotator rotation = tangent.Rotation();
				rotation += FRotator(0, 0, random.FRandRange(0, 360));
				mc->SetRelativeRotation(rotation);

				if (LeafStaticMesh) {
					mc->SetStaticMesh(LeafStaticMesh);
				}
				if (LeafMaterial) {
					mc->SetMaterial(0, LeafMaterial);
				}
			}

			mc->SetRelativeScale3D(FVector(LeafScale));
		}

		// ensure spline is updated
		constexpr bool bUpdateSpline = true;

		// attach spline component to root
		FAttachmentTransformRules Rules(FAttachmentTransformRules::KeepRelativeTransform);
		SplineComponent->AttachToComponent(RootComponent, Rules);
	}
}

void ATree::ClearTree() {
	TArray smcs = this->GetComponentsByTag(USplineComponent::StaticClass(), TAG_Branch);
	TArray splines = this->GetComponentsByTag(USplineComponent::StaticClass(), TAG_Spline);
	
	for (auto& smc : smcs) {
		smc->UnregisterComponent();
		smc->DestroyComponent();
	}

	for (auto& spline : splines) {
		spline->UnregisterComponent();
		spline->DestroyComponent();
	}

	states.Empty();
	age = 0;
	simulator.clusters.Empty();
}

void ATree::AssignTreeMaterial() {
	if (TreeMaterial) {
		TArray<UActorComponent*> components = GetComponentsByTag(USplineMeshComponent::StaticClass(), TAG_Branch);
		for (UActorComponent* component : components)
		{
			Cast<USplineMeshComponent>(component)->SetMaterial(0, TreeMaterial);
		}
	}
}

void ATree::AssignTreeMesh() {
	if (TreeStaticMesh) {
		TArray<UActorComponent*> components = GetComponentsByTag(USplineMeshComponent::StaticClass(), TAG_Branch);
		for (UActorComponent* component : components)
		{
			Cast<USplineMeshComponent>(component)->SetStaticMesh(TreeStaticMesh);
		}
	}
}

void ATree::AssignLeafMaterial() {
	if (LeafMaterial) {
		TArray<UActorComponent*> components = GetComponentsByTag(UStaticMeshComponent::StaticClass(), TAG_Leaves);
		for (UActorComponent* component : components)
		{
			Cast<UStaticMeshComponent>(component)->SetMaterial(0, LeafMaterial);
		}
	}
}

void ATree::AssignLeafMesh() {
	if (LeafStaticMesh) {
		TArray<UActorComponent*> components = GetComponentsByTag(UStaticMeshComponent::StaticClass(), TAG_Leaves);
		for (UActorComponent* component : components)
		{
			Cast<UStaticMeshComponent>(component)->SetStaticMesh(LeafStaticMesh);
		}
	}
}

void ATree::UpdateBranchThickness(USplineMeshComponent* smc, const Branch& branch, int nodeIdx, int treeHeight) {
	check(smc != nullptr);

	// Reduce thickness of the branch along its length.
	float startThicknessFactor = ((float)branch.rootTreeDepth + nodeIdx) / treeHeight;
	float endThicknessFactor = ((float)branch.rootTreeDepth + nodeIdx + 1) / treeHeight;

	float startThickness = FMath::Lerp(RootBranchThickness, LastBranchThickness, startThicknessFactor);
	float endThickness = FMath::Lerp(RootBranchThickness, LastBranchThickness, endThicknessFactor);

	// Reduce entire branch thickness - child branches are always thinner that their roots.
	startThickness = FMath::Pow(startThickness, 1.0f / BranchLevelThicknessFactor);
	endThickness = FMath::Pow(endThickness, 1.0f / BranchLevelThicknessFactor);

	// Apply scale to the branch.
	smc->SetStartScale(FVector2D(startThickness));
	smc->SetEndScale(FVector2D(endThickness));
}
