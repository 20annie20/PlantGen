// Fill out your copyright notice in the Description page of Project Settings.


#include "Branch.h"

Branch::Branch()
{
	nodes.AddDefaulted();
	state = ACTIVE;
}

// TODO - update this function for lateral buds creation
Branch::Branch(FVector coords, FQuat rotation, int level, int numBuds)
{
	nodes.Add(Node(coords, rotation, true, level, numBuds));
	state = ACTIVE;
}

Branch::~Branch()
{
}
