// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Branch.h"
#include "Species.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

struct State;

/**
 * 
 */

class TREEGEN2_API ParametricSimulator
{
	Species species;
	int maxBranchLevel;
	TArray<FVector> clusters;
	const UWorld* world;

	TArray<Node> CalculateNextBuds(const Node last_node, const int age);
	bool checkState(const float probabilityOfDeath);
	bool checkApicalGrowth(FVector coords);
	bool checkLateralGrowth(FVector coords);
	float computeIllumination(FVector coords);
	float randAngle(const int angleMean, const int angleVariance);
	FVector Bend(FVector vector, float angleRad);
	FVector Roll(FVector vector, float angleRad);

public:
	ParametricSimulator();
	ParametricSimulator(Species_ID species_id, const UWorld* world);
	~ParametricSimulator();
	State GrowTree(const State& state, int age, bool useSDF);
};
