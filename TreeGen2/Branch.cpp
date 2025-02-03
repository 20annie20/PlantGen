// Fill out your copyright notice in the Description page of Project Settings.


#include "Branch.h"

Branch::Branch()
{
	nodes.Add(Node(FVector(0,0,0), FVector(0,0,1), true, 0, 0));
	state = ACTIVE;
}

// TODO - update this function for lateral buds creation
Branch::Branch(FVector coords, FVector heading, int level, int numBuds)
{
	nodes.Add(Node(coords, heading, true, level, numBuds));
	state = ACTIVE;
}

Branch::~Branch()
{
}
