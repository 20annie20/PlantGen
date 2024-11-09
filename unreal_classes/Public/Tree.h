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
	int AAV;  // apical angle variance
	float GR; // growth rate
	float ILB; // internode length base
};

const TMap<Species, SpeciesParams> speciesTable = {
	{
		Species::ACACIA, {
			2,			// AAV
			1.5,		// GR
			0.92		// ILB
		}
	},
	{
		Species::APPLE, {
			20,			// AAV
			2.98,		// GR
			0.55		// ILB
		}
	},
	{
		Species::WILLOW, {
			12,			// AAV
			2.3,		// GR
			0.8			// ILB

		}
	},
	{
		Species::MAPLE, {
			2,			// AAV
			2.3,		// GR
			0.8			// ILB
		}
	},
	{
		Species::BIRCH, {
			5,			// AAV
			4.25,		// GR
			0.93		// ILB
		}
	
	},
	{
		Species::OAK, {
			20,			// AAV
			3,			// GR
			1			// ILB
		}
	},
	{
		Species::PINE, {
			0,			// AAV
			3.26,		// GR
			0.4			// ILB
		}
	}
};

class Branch {
	TArray<FVector> nodes = {FVector(0.0)};
}

UCLASS()
class TREEGENPLUGIN_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	ATree();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetAxiom"))
	FString Axiom = "X";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetIterations"))
	int Iterations = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetAngle"))
	float Angle = 22.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetRadius"))
	float Radius = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetLength"))
	float Length = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection,
		meta = (BlueprintSetter = "SetBranchRadiusReduction"))
	float BranchRadiusReduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetMinBranchRadius"))
	float MinBranchRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection,
		meta = (BlueprintSetter = "SetBranchLengthRelativeToParent"))
	float BranchLengthRelativeToParent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection, meta = (BlueprintSetter = "SetLeafSize"))
	float LeafSize;

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
	void SetAngle(float NewAngle);

	UFUNCTION(BlueprintSetter)
	void SetAxiom(FString NewAxiom);

	UFUNCTION(BlueprintSetter)
	void SetIterations(int NewIterations);

	UFUNCTION(BlueprintSetter)
	void SetRadius(float NewRadius);

	UFUNCTION(BlueprintSetter)
	void SetLength(float NewLength);

	UFUNCTION(BlueprintSetter)
	void SetBranchRadiusReduction(float NewBranchRadiusReduction);

	UFUNCTION(BlueprintSetter)
	void SetMinBranchRadius(float NewMinBranchRadius);

	UFUNCTION(BlueprintSetter)
	void SetBranchLengthRelativeToParent(float NewBranchLengthRelativeToParent);

	UFUNCTION(BlueprintSetter)
	void SetLeafSize(float NewLeafSize);

protected:
	virtual void PostInitializeComponents() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//virtual void BeginPlay() override;

	void GenerateTree();

	void RefreshTree();

	void PopulateMesh();

	//FInstanceData CreateInstanceData(const FTransform& Transform, TEnumAsByte<EInstanceType> InstanceType);

private:
	TArray<USplineComponent*> splines;
	//FTransform TransformToGlobal(const FTransform& Parent, const FTransform& Local);
};
