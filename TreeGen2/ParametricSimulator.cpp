// Fill out your copyright notice in the Description page of Project Settings.


#include "ParametricSimulator.h"
#include "Tree.h"
#include <random>
#include "Kismet/KismetMathLibrary.h"

std::random_device rd{};
std::mt19937 gen{ rd() };

ParametricSimulator::ParametricSimulator()
{
	this->species = Species(MAPLE);
	this->maxBranchLevel = 0;
}

ParametricSimulator::ParametricSimulator(Species_ID species_id)
{
	this->species = Species(species_id);
	this->maxBranchLevel = 1;
}

ParametricSimulator::~ParametricSimulator()
{
}

State ParametricSimulator::GrowTree(const State& state, int age, bool useSDF)
{
	State newState = State();
	newState.branches.Empty();
	FVector next_bud_position;

	for (int32 idx = 0; idx < state.branches.Num(); idx++) {
		TArray<Branch> newBranches;
		newState.branches.Add(state.branches[idx]);

		// check to kill apical bud
		if (species.apicalExtinctionRate > 0.0) {
			bool doesDie = checkState(species.apicalExtinctionRate);
			if (doesDie) {
				newState.branches[idx].state = STALE; // change the state to STALE
				newState.branches[idx].nodes.Last().isAlive = false; // kill the bud
				continue;
			}
		}

		for (int node_idx = 0; node_idx < newState.branches[idx].nodes.Num(); node_idx++) {
			auto newNumBuds = newState.branches[idx].nodes[node_idx].numLatBuds;

			// check to kill lateral buds
			for (int bud_idx = 0; bud_idx < newState.branches[idx].nodes[node_idx].numLatBuds; bud_idx++) {
				if (checkState(species.lateralExtinctionRate)) 
					newNumBuds--;
			}
			newState.branches[idx].nodes[node_idx].numLatBuds = newNumBuds;

			// lateral shoot growth with probability
			for (int bud_idx = 0; bud_idx < newState.branches[idx].nodes[node_idx].numLatBuds; bud_idx++) {
				if (checkLateralGrowth()) {
					// newBranches.Add(Branch());
					// TODO - modify new branch's heading, using BAM and RAM
				}
			}
		}

		// grow apically only for living branches
		if (state.branches[idx].nodes.Last().isAlive) {
			// TODO - add probability of this growth
			if (checkApicalGrowth()) {
				// grow a new apical shoot
				auto nextBuds = CalculateNextBuds(newState.branches[idx].nodes.Last(), age);

				for (auto& bud : nextBuds) {
					newState.branches[idx].nodes.Add(bud);
				}
			}
		}
	}

	return newState;
}

TArray<Node> ParametricSimulator::CalculateNextBuds(const Node previous, const int age) {
	
	// calculate shoot length
	float actualGrowthRate;
	float actualApicalControl = species.apicalControl * FMath::Pow(species.apicalControlAgeFactor, age);
	if (actualApicalControl > 1) {
		actualGrowthRate = species.initialGrowthRate / FMath::Pow(actualApicalControl, previous.levelOfBranch);
	}
	else {
		actualGrowthRate = species.initialGrowthRate / FMath::Pow(actualApicalControl, maxBranchLevel);
	}
		
	float actualInternodeLength = species.internodeBaseLength * FMath::Pow(species.internodeLengthAgeFactor, age);
	int numOfInternodes = FMath::RoundToInt(actualGrowthRate);
	float shootLength = actualInternodeLength * 100.0;

	// apical angle variance
	FVector2D polar_heading;
	std::normal_distribution d{0.0f, (float)UKismetMathLibrary::DegreesToRadians(species.apicalAngleVariance) };

	polar_heading.X = d(gen);
	polar_heading.Y = (float)UKismetMathLibrary::DegreesToRadians(FMath::RandRange(0.0, 360.0));
	auto var = polar_heading.SphericalToUnitCartesian();

	auto current_heading = previous.heading + var;	
	TArray<Node> nodes;

	auto coords = previous.coordinates;

	for (int i = 0; i < numOfInternodes; i++) {
		auto newNode = Node(coords + current_heading * shootLength, current_heading, true, previous.levelOfBranch, species.numOfLateralBuds);
		coords = newNode.coordinates;
		nodes.Add(newNode);
	}
	return nodes;
}

bool ParametricSimulator::checkState(const float probabilityOfDeath) {
	std::uniform_real_distribution d{ 0.0f, 1.0f };
	auto prob = d(gen);
	return prob < probabilityOfDeath;
}

bool ParametricSimulator::checkApicalGrowth() {
	float illumination = computeIllumination();

	auto prob = checkState(FMath::Pow(illumination, species.apicalLightFactor));

	// TODO return prob
	return true;
}

bool ParametricSimulator::checkLateralGrowth() {
	
	float illumination = computeIllumination();

	// TODO sum for each bud (b_j) above given (b_i):
	// delta_j * ad_bf * FMath::Pow(ad_af, age) * FMath::Pow(ad_df, branch_wise_distance between b_i and b_j)
	float sum = 0.0f;

	float exponential = FMath::Exp(-sum);

	auto prob = checkState(FMath::Pow(illumination, species.lateralLightFactor) * exponential);

	// TODO return prob
	return true;
}

float ParametricSimulator::computeIllumination() {
	// TODO -  It is computed using the light model[PSK * 12] 
	return 1.0f;
}