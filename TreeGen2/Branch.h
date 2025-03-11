// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

enum BranchState {
	ACTIVE = 1,
	STALE = 0
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

	Branch();
	Branch(FVector coords, FQuat rotation, int level, int numBuds);

	~Branch();
};
