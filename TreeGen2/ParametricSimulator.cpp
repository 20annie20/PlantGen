// Fill out your copyright notice in the Description page of Project Settings.


#include "ParametricSimulator.h"
#include "Tree.h"
#include "TreeGen2.h"
#include <random>
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"   // Include the DrawDebugHelpers header

#define CLUSTER_RADIUS 10.0F
#define NODE_SCALE 40.0F

std::random_device rd{};
std::mt19937 gen{ rd() };

ParametricSimulator::ParametricSimulator()
{
	this->species = Species(MAPLE);
	this->maxBranchLevel = 0;
	this->clusters.Empty();
}

ParametricSimulator::ParametricSimulator(Species_ID species_id, const UWorld* world)
{
	this->species = Species(species_id);
	this->maxBranchLevel = 1;
	this->clusters.Empty();
	this->world = world;
}

ParametricSimulator::~ParametricSimulator()
{
}

State ParametricSimulator::GrowTree(const State& state, int age, bool useSDF)
{
	State newState;

	for (int32 idx = 0; idx < state.branches.Num(); idx++) {
		Branch branch = state.branches[idx];

		// check to kill apical bud
		if (species.apicalExtinctionRate > 0.0) {
			bool doesDie = checkState(species.apicalExtinctionRate);
			if (doesDie) {
				branch.state = STALE;
				branch.nodes.Last().isAlive = false; // kill the bud
				newState.branches.Add(std::move(branch));
				continue;
			}
		}

		TArray<Branch> newBranches;

		for (int node_idx = 0; node_idx < branch.nodes.Num(); node_idx++) {
			auto& node = branch.nodes[node_idx];

			// check to kill lateral buds
			int newNumBuds = node.numLatBuds;
			for (int bud_idx = 0; bud_idx < node.numLatBuds; bud_idx++) {
				if (checkState(species.lateralExtinctionRate)) 
					newNumBuds--;
			}
			node.numLatBuds = newNumBuds;

			// lateral shoot growth with probability
			// TODO - should it be considered separately per bud???
			bool growLateral = false;
			if (node.numLatBuds > 0) {
				growLateral = checkLateralGrowth(node.coordinates, state, age, node.levelOfBranch);
			}

			for (int bud_idx = 0; bud_idx < node.numLatBuds; bud_idx++) {
				if (growLateral) {
					
					float rollAngle = (360.f / node.numLatBuds) * bud_idx + randAngle(species.rollAngleMean, species.rollAngleVariance);
					float bendAngle = randAngle(species.bendingAngleMean, species.bendingAngleVariance);

					FQuat localRotation(FRotator(bendAngle, rollAngle, 0));
					FQuat newRotation = (node.orientation * localRotation);

					newBranches.Emplace(
						node.coordinates,
						newRotation,
						node.levelOfBranch + 1,
						0);

					FVector newHeading = newRotation.RotateVector(FVector::UpVector);
					UE_LOG(TreeGenLog, Log,
						TEXT("New branch created from bud at [%f, %f, %f] heading [%f, %f, %f] at level %d, roll=%f bend=%f"),
						node.coordinates.X, node.coordinates.Y, node.coordinates.Z,
						newHeading.X, newHeading.Y, newHeading.Z,
						node.levelOfBranch + 1,
						rollAngle,
						bendAngle);

					DrawDebugLine(
						world,
						node.coordinates,
						node.coordinates + (newHeading * 10),
						FColor(0, 255, 0),
						true);
				}
			}

			node.numLatBuds -= newBranches.Num();
		}

		// grow apically only for living branches
		if (branch.nodes.Last().isAlive) {
			bool doesGrow = checkApicalGrowth(branch.nodes.Last().coordinates);
			if (doesGrow) {
				// grow a new apical shoot
				branch.nodes.Append(CalculateNextBuds(branch.nodes.Last(), age));
			}
		}

		newState.branches.Add(std::move(branch));
		newState.branches.Append(std::move(newBranches));
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
	float shootLength = actualInternodeLength * NODE_SCALE;

	// apical angle variance
	//FVector2D polar_heading;
	//std::normal_distribution d{0.0f, (float)UKismetMathLibrary::DegreesToRadians(species.apicalAngleVariance) };
	std::normal_distribution d{ 0.f, (float)species.apicalAngleVariance };

	//polar_heading.X = d(gen);
	//polar_heading.Y = (float)UKismetMathLibrary::DegreesToRadians(FMath::RandRange(0.0, 360.0));
	//auto var = polar_heading.SphericalToUnitCartesian();
	

	TArray<Node> nodes;
	for (int i = 0; i < numOfInternodes; i++) {
		FQuat varOrientation(FRotator(d(gen), FMath::RandRange(0, species.apicalAngleVariance), 0));
		FQuat currentOrientation = (previous.orientation * varOrientation);

		FVector currentHeading = currentOrientation.RotateVector(FVector::UpVector);
		FVector coords = previous.coordinates;
		coords += (currentHeading * shootLength);

		UE_LOG(TreeGenLog, Log,
			TEXT("New internode created at [%f, %f, %f] heading [%f, %f, %f] at level %d"),
			coords.X, coords.Y, coords.Z,
			currentHeading.X, currentHeading.Y, currentHeading.Z,
			previous.levelOfBranch);

		nodes.Emplace(
			coords,
			currentOrientation,
			true,
			previous.levelOfBranch,
			species.numOfLateralBuds);

		clusters.Add(coords);
	}

	return nodes;
}

bool ParametricSimulator::checkState(const float probabilityOfDeath) {
	std::uniform_real_distribution d{ 0.0f, 1.0f };
	auto prob = d(gen);
	return prob < probabilityOfDeath;
}

bool ParametricSimulator::checkApicalGrowth(FVector coords) {
	float illumination = computeIllumination(coords);

	auto doesGrow = checkState(FMath::Pow(illumination, species.apicalLightFactor));

	return doesGrow;
}

bool ParametricSimulator::checkLateralGrowth(const FVector coords, const State& state, const int age, const int level) {
	
	float illumination = computeIllumination(coords);
	float sum = 0.0f;

	float distance = 1.0f;
	// TODO compute distance between buds

	// TODO change state to Z-wise sorted list of coords
	for (int i = 0; i < state.branches.Num(); i++) {
		// if branch starts above coords - this means it is a flushed bud
		if (state.branches[i].nodes[0].coordinates.Z > coords.Z) { // skip branches that start below or the same point
			distance = state.branches[i].nodes[0].levelOfBranch;
			sum += species.apicalDominanceBase * FMath::Pow(species.apicalDominanceAge, age) * FMath::Pow(species.apicalDominanceDistance, distance);
		}
	}

	float exponential = FMath::Exp(-sum);

	auto doesGrow = checkState(FMath::Pow(illumination, species.lateralLightFactor) * exponential);

	return doesGrow;
}

float ParametricSimulator::computeIllumination(FVector coords) {
	int rayDensity = 4;
	auto heading0 = FVector(1, 0, 0);
	auto heading45 = Bend(heading0, UKismetMathLibrary::DegreesToRadians(45));
	auto heading80 = Bend(heading0, UKismetMathLibrary::DegreesToRadians(80));

	FVector rayHeading0, rayHeading45, rayHeading80;
	int miss = 0;
	for (int i = 0; i < rayDensity; i++) {
		auto angle = float(i) * ( 2 * PI / rayDensity);

		// TODO - fix repetition when im not so lazy
		rayHeading0 = Roll(heading0, angle);
		rayHeading45 = Roll(heading45, angle);
		rayHeading80 = Roll(heading80, angle);

		if (traceRayMiss(coords, rayHeading0)) {
			miss++;
			/*DrawDebugLine(
				world,
				coords,
				coords + (rayHeading0 * 20),
				FColor(255, 0, 255),
				true
			);*/
		}
		else {
			/*DrawDebugLine(
				world,
				coords,
				coords + (rayHeading0 * 20),
				FColor(255, 255, 0),
				true
			);*/
		}

		if (traceRayMiss(coords, rayHeading45)) {
			miss++;
			/*DrawDebugLine(
				world,
				coords,
				coords + (rayHeading45 * 20),
				FColor(255, 0, 255),
				true
			);*/
		}
		else {
			/*DrawDebugLine(
				world,
				coords,
				coords + (rayHeading45 * 20),
				FColor(255, 255, 0),
				true
			);*/
		}

		if (traceRayMiss(coords, rayHeading80)) {
			miss++;
			/*DrawDebugLine(
				world,
				coords,
				coords + (rayHeading80 * 20),
				FColor(255, 0, 255),
				true
			);*/
			
		}
		else {
			/*DrawDebugLine(
				world,
				coords,
				coords + (rayHeading80 * 20),
				FColor(255, 255, 0),
				true
			);*/
		}
	}

	return  float(miss) / (rayDensity * 3);
}

float ParametricSimulator::sdSphere(FVector p) {
	return p.Length() - CLUSTER_RADIUS;
};

bool ParametricSimulator::raymarchHit(FVector rayOrigin, FVector direction, FVector sphereCenter) {

	// TODO - add BVH
	float dO = 0.0;
	auto MAX_STEPS = 1000;
	auto MAX_DIST = 1000.0;
	auto SURFACE_DIST = 0.01f;

	for (int i = 0; i < MAX_STEPS; i++) {
		auto p = rayOrigin + direction * dO;
		float dS = sdSphere(p - sphereCenter);

		dO += dS;

		if (dS < SURFACE_DIST) {
			return true;
		}
		if (dO > MAX_DIST) {
			return false;
		}
	}
	return false;
}

bool ParametricSimulator::traceRayMiss(const FVector& coords, const FVector& direction) {
	for (int i = 0; i < clusters.Num(); i++)
	{
		if (clusters[i].Z < coords.Z || clusters[i].Equals(coords)) { // skip clusters that are below or the same point
			continue;
		}

		if (raymarchHit(coords, direction, clusters[i]))
			return false;
	}

	return true;
}

float ParametricSimulator::randAngle(const int angleMean, const int angleVariance) {
	std::uniform_int_distribution d{ angleMean - angleVariance, angleMean + angleVariance };
	//return UKismetMathLibrary::DegreesToRadians(d(gen));
	return d(gen);
}

FVector ParametricSimulator::Bend(FVector vector, float angleRad) {
	auto newVector = vector;
	newVector.X = vector.Z * cos(angleRad) - vector.X * sin(angleRad);
	newVector.Z = vector.Z * sin(angleRad) + vector.X * cos(angleRad);
	return newVector;
}

FVector ParametricSimulator::Roll(FVector vector, float angleRad) {
	auto newVector = vector;
	newVector.X = vector.Y * cos(angleRad) - vector.X * sin(angleRad);
	newVector.Y = vector.Y * sin(angleRad) + vector.X * cos(angleRad);
	return newVector;
}