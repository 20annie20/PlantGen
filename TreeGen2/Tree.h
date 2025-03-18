// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParametricSimulator.h"
#include "Species.h"
#include "Branch.h"
#include "Tree.generated.h"

class USplineMeshComponent;

struct State {
	TArray<Branch> branches;

	int CalcHeight(int startIdx) const;
};

UCLASS()
class TREEGEN2_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATree();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	int32 age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	TEnumAsByte<Species_ID> species_id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	UMaterial* TreeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	UStaticMesh* TreeStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
	UMaterial* LeafMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
	UStaticMesh* LeafStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	float RootBranchThickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	float LastBranchThickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
	float BranchLevelThicknessFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
	float LeafScale;

	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	
protected:

	ParametricSimulator simulator;

	// Called when the game starts or when spawned in runtime
	virtual void BeginPlay() override;

	// Grow tree from the start
	void GenerateTree();

	void RefreshMesh(const State& state);

	void ClearTree();

	void AssignTreeMaterial();
	void AssignTreeMesh();
	void AssignLeafMaterial();
	void AssignLeafMesh();

	void UpdateBranchThickness(USplineMeshComponent* smc, const Branch& branch, int nodeIdx, int treeHeight);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<State> states;
};
