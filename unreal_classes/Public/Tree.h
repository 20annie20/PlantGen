// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Tree.generated.h"

// instancing inspo from https://github.com/claitz/UE-TreeGenerator/blob/master/Source/Tree/Public/Actors/TreeActor.h
UENUM(BlueprintType)
enum EInstanceType
{
	EIT_Trunk UMETA(DisplayName = "Branch"),
	EIT_Leaf UMETA(DisplayName = "Leaf")
};

UENUM(BlueprintType)
enum Species
{
	ACACIA	UMETA(DisplayName = "Acacia"),
	APPLE   UMETA(DisplayName = "Apple"),
	WILLOW  UMETA(DisplayName = "Willow"),
	MAPLE   UMETA(DisplayName = "Maple"),
	BIRCH   UMETA(DisplayName = "Birch"),
	OAK		UMETA(DisplayName = "Oak"),
	PINE	UMETA(DisplayName = "Pine")
};

struct SpeciesParams {
	int NLB;	// number of lateral buds
	int AAV;	// apical angle variance
	int BAM;	// branching angle mean
	int BAV;	// branching angle variance
	int RAM;	// roll angle mean
	int RAV;	// roll angle variance

	float GR;	// growth rate
	float ILB;	// internode length base
};

const TMap<Species, SpeciesParams> speciesTable = {
	{
		Species::ACACIA, {
			2,			// NLB
			2,			// AAV
			20,			// BAM
			5,			// BAV
			113,		// RAM
			13,			// RAV
			1.5,		// GR
			0.92		// ILB
		}
	},
	{
		Species::APPLE, {
			2,			// NLB
			20,			// AAV
			45,			// BAM
			2,			// BAV
			91,			// RAM
			1,			// RAV
			2.98,		// GR
			0.55		// ILB
		}
	},
	{
		Species::WILLOW, {
			2,			// NLB
			12,			// AAV
			43,			// BAM
			3,			// BAV
			80,			// RAM
			4,			// RAV
			2.3,		// GR
			0.8			// ILB

		}
	},
	{
		Species::MAPLE, {
			2,			// NLB
			2,			// AAV
			30,			// BAM
			5,			// BAV
			130,		// RAM
			30,			// RAV
			2.3,		// GR
			0.8			// ILB
		}
	},
	{
		Species::BIRCH, {
			2,			// NLB
			5,			// AAV
			30,			// BAM
			5,			// BAV
			130,			// RAM
			30,			// RAV
			4.25,		// GR
			0.93		// ILB
		}
	
	},
	{
		Species::OAK, {
			3,			// NLB
			20,			// AAV
			29,			// BAM
			2,			// BAV
			91,			// RAM
			1,			// RAV
			3,			// GR
			1			// ILB
		}
	},
	{
		Species::PINE, {
			3,			// NLB
			0,			// AAV
			80,			// BAM
			4,			// BAV
			10,			// RAM
			30,			// RAV
			3.26,		// GR
			0.4			// ILB
		}
	}
};

enum BranchState {
	ACTIVE = 1,
	STALE = 0
};

class Branch {
public:
	TArray<FVector> nodes;
	int nodes_count;
	BranchState state;
	Branch* parent;
	bool is_terminal;
	float start_width, end_width;

	Branch();
	Branch(Branch* parent);
	void AddNode();
};

UCLASS()
class TREEGENPLUGIN_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	ATree();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetSpecies"))
	TEnumAsByte<Species> species;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	UMaterial* TreeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	UStaticMesh* TreeStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
	UMaterial* LeafMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
	UStaticMesh* LeafStaticMesh;

	int ChunkSize = 100;
	int CurrentChunk = 0;
	
	UFUNCTION(BlueprintSetter)
	void SetSpecies(Species NewAngle);

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	//virtual void BeginPlay() override;

	void GenerateTree();

	void RefreshTree();

	void PopulateMesh();

	//FInstanceData CreateInstanceData(const FTransform& Transform, TEnumAsByte<EInstanceType> InstanceType);

private:
	TMap<USplineComponent*, Branch*> splines;

	TArray<Branch> grown_branches;
	//FTransform TransformToGlobal(const FTransform& Parent, const FTransform& Local);
};
