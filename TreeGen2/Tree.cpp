// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto world = GetWorld();
	simulator = ParametricSimulator(species_id, world);
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

	// TODO do not create new component if such component already exists - check smh
	// TODO clear splines completely in the clearTree() method
	for (int32 idx = 0; idx < state.branches.Num(); idx++) {
		auto branch_name = FName(FString::FormatAsNumber(idx) + "_spline");
		USplineComponent* SplineComponent = (USplineComponent*)GetDefaultSubobjectByName(branch_name);
		if (SplineComponent == NULL) {
			SplineComponent = NewObject<USplineComponent>(this, branch_name);
			SplineComponent->SetMobility(EComponentMobility::Static);
			SplineComponent->RegisterComponent();
			SplineComponent->ComponentTags.Add(TEXT("Spline"));
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

			FVector start_location, start_tangent;

			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(n_idx, start_location, start_tangent);

			// assign mesh to each spline point
			FName name = FName(SplineComponent->GetFName().ToString() + "_" + FString::FormatAsNumber(n_idx) + "_smc");
			USplineMeshComponent* smc = (USplineMeshComponent*)GetDefaultSubobjectByName(name);
			
			if (smc == NULL) {
				smc = NewObject<USplineMeshComponent>(this, name);
				smc->SetMobility(EComponentMobility::Static);
				smc->SetForwardAxis(ESplineMeshAxis::Type::Z);
				smc->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
				smc->ComponentTags.Add(TEXT("SMC"));

				// TODO WA the end_tangent problem && clean the meshes from the smcs on the state restore 
				smc->SetStartAndEnd(state.branches[idx].nodes[n_idx].coordinates, start_tangent, state.branches[idx].nodes[n_idx+1].coordinates, start_tangent);
				if (TreeStaticMesh) {
					smc->SetStaticMesh(TreeStaticMesh);
				}
				if (TreeMaterial) {
					smc->SetMaterial(0, TreeMaterial);
				}
			}
		}

		// ensure spline is updated
		constexpr bool bUpdateSpline = true;
		// attach spline component to root
		FAttachmentTransformRules Rules(FAttachmentTransformRules::KeepRelativeTransform);
		SplineComponent->AttachToComponent(RootComponent, Rules);
	}
}

void ATree::ClearTree() {
	TArray smcs = this->GetComponentsByTag(USplineComponent::StaticClass(), FName("SMC"));
	TArray splines = this->GetComponentsByTag(USplineComponent::StaticClass(), FName("Spline"));
	
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
		TSet comps = this->GetComponents();

		for (UActorComponent* component : comps)
		{
			if (component->ComponentHasTag(TEXT("SMC"))) {
				UStaticMeshComponent* smc = Cast<UStaticMeshComponent>(component);
				if (smc)
				{
					smc->SetMaterial(0, TreeMaterial);
				}
			}
		}
	}
}

void ATree::AssignTreeMesh() {
	if (TreeStaticMesh) {
		TSet comps = this->GetComponents();

		for (UActorComponent* component : comps)
		{
			if (component->ComponentHasTag(TEXT("SMC"))) {
				UStaticMeshComponent* smc = Cast<UStaticMeshComponent>(component);
				if (smc)
				{
					smc->SetStaticMesh(TreeStaticMesh);
				}
			}
		}
	}
}