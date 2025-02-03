// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Branch.h"
#include "Species.h"

struct State;

/**
 * 
 */

class TREEGEN2_API ParametricSimulator
{
	Species species;
	int maxBranchLevel;
	TArray<Node> CalculateNextBuds(const Node last_node, const int age);
	bool checkState(const float probabilityOfDeath);
	bool checkApicalGrowth();
	bool checkLateralGrowth();
	float computeIllumination();
public:
	ParametricSimulator();
	ParametricSimulator(Species_ID species_id);
	~ParametricSimulator();
	State GrowTree(const State& state, int age, bool useSDF);
};
