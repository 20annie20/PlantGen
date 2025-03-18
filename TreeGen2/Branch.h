// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

enum class BranchState {
	Active = 1,
	Stale = 0
};

struct Node {
	FVector coordinates = FVector::ZeroVector;
	FQuat orientation = FQuat::Identity;
	bool isAlive = true;
	int levelOfBranch = 0;
	int numLatBuds = 0;
};

class TREEGEN2_API Branch
{
public:
	TArray<Node> nodes;
	BranchState state;

	// Number of nodes between the root of this branch and the root of the entire tree.
	// Used to calculate thickness of the branch at its base.
	int rootTreeDepth;

	// Number of branches growing out of this branch.
	int numChildBranches;

	Branch();
	Branch(FVector coords, FQuat rotation, int level, int numBuds, int rootTreeDepth);

	~Branch();
};
