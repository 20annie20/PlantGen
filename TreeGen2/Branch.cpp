// Fill out your copyright notice in the Description page of Project Settings.


#include "Branch.h"

Branch::Branch()
	: state(BranchState::Active)
	, rootTreeDepth(0)
	, numChildBranches(0)
{
	nodes.AddDefaulted();
}

// TODO - update this function for lateral buds creation
Branch::Branch(FVector coords, FQuat rotation, int level, int numBuds, int rootTreeDepth)
	: state(BranchState::Active)
	, rootTreeDepth(rootTreeDepth)
	, numChildBranches(0)
{
	nodes.Add(Node(coords, rotation, true, level, numBuds));
}

Branch::~Branch()
{
}
