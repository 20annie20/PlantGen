#pragma once

#include "CoreMinimal.h"
#include "Tree.h"

class ParametricSystem
{
	Species species;
	TArray<Branch> branches;
	float width_reduction_factor;
	int tree_age;

public:
	ParametricSystem(TEnumAsByte<Species> species, int tree_age);
	TArray<Branch> GrowBranches();

private:
	void CalculateCoords();
	TArray<Branch> MergeLists();
};
