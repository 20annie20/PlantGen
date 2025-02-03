// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

enum BranchState {
	ACTIVE = 1,
	STALE = 0
};

struct Node {
	FVector coordinates;
	FVector heading;
	bool isAlive;
	int levelOfBranch;
	int numLatBuds;
};

class TREEGEN2_API Branch
{
public:
public:
	TArray<Node> nodes;
	BranchState state;
	Branch();
	Branch(FVector coords, FVector heading, int level, int numBuds);
	~Branch();
};
