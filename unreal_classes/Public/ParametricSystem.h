#pragma once

#include "CoreMinimal.h"
#include "Tree.h"

class ParametricSystem
{
	Species species;
	TArray<Branch> branches;
	float width_reduction_factor;

public:
	ParametricSystem(TEnumAsByte<Species> species);
	TArray<Branch> GrowBranches();

private:
	void CalculateCoords();
	TArray<Branch> MergeLists();
};