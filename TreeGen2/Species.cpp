// Fill out your copyright notice in the Description page of Project Settings.


#include "Species.h"

Species::Species() {

};

Species::~Species() {

};

Species::Species(Species_ID newId)
{
	this->id = newId;
	if (id == ACACIA) {
		this->initialGrowthRate = 1.5f;
		this->apicalControl = 2.4f;
		this->apicalControlAgeFactor = 0.92f;
		this->internodeBaseLength = 0.92f;
		this->internodeLengthAgeFactor = 0.96f;
		this->apicalAngleVariance = 2;

		this->bendingAngleMean = 20;
		this->bendingAngleVariance = 5;
		this->rollAngleMean = 113;
		this->rollAngleVariance = 13;

		this->apicalExtinctionRate = 0.9f;
		this->lateralExtinctionRate = 0.006f;
		this->numOfLateralBuds = 2;
		this->apicalLightFactor = 1.0f;
		this->lateralLightFactor = 1.0f;
		this->apicalDominanceBase = 3.5f;
		this->apicalDominanceDistance = 0.9f;
		this->apicalDominanceAge = 0.9f;
	}
	if (id == APPLE) {
		this->initialGrowthRate = 2.98f;
		this->apicalControl = 2.2f;
		this->apicalControlAgeFactor = 0.5f;
		this->internodeBaseLength = 0.55f;
		this->internodeLengthAgeFactor = 0.97f;
		this->apicalAngleVariance = 20;

		this->bendingAngleMean = 45;
		this->bendingAngleVariance = 2;
		this->rollAngleMean = 91;
		this->rollAngleVariance = 1;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.21f;

		this->numOfLateralBuds = 2;

		this->apicalLightFactor = 0.39f;
		this->lateralLightFactor = 1.13f;

		this->apicalDominanceBase = 3.13f;
		this->apicalDominanceDistance = 0.13f;
		this->apicalDominanceAge = 0.82f;
	}
	if (id == WILLOW) {
		this->initialGrowthRate = 2.3f;
		this->apicalControl = 3.25f;
		this->apicalControlAgeFactor = 0.7f;
		this->internodeBaseLength = 0.8f;
		this->internodeLengthAgeFactor = 0.98f;
		this->apicalAngleVariance = 12; 
		
		this->bendingAngleMean = 42;
		this->bendingAngleVariance = 2;
		this->rollAngleMean = 80;
		this->rollAngleVariance = 4;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.21f;

		this->numOfLateralBuds = 2;

		this->apicalLightFactor = 0.54f;
		this->lateralLightFactor = 0.94f;

		this->apicalDominanceBase = 0.38f;
		this->apicalDominanceDistance = 0.9f;
		this->apicalDominanceAge = 0.31f;
	}
	if (id == MAPLE) {
		this->initialGrowthRate = 4.25f;
		this->apicalControl = 3.64f;
		this->apicalControlAgeFactor = 0.87f;
		this->internodeBaseLength = 0.93f;
		this->internodeLengthAgeFactor = 0.95f;
		this->apicalAngleVariance = 2;

		this->bendingAngleMean = 30;
		this->bendingAngleVariance = 5;
		this->rollAngleMean = 130;
		this->rollAngleVariance = 30;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.01f;

		this->numOfLateralBuds = 2;

		this->apicalLightFactor = 0.85f;
		this->lateralLightFactor = 0.2f;

		this->apicalDominanceBase = 4.87f;
		this->apicalDominanceDistance = 0.98f;
		this->apicalDominanceAge = 0.42f;
	}
	if (id == BIRCH) {
		this->initialGrowthRate = 4.25f;
		this->apicalControl = 3.64f;
		this->apicalControlAgeFactor = 0.89f;
		this->internodeBaseLength = 0.93f;
		this->internodeLengthAgeFactor = 0.93f;
		this->apicalAngleVariance = 5;

		this->bendingAngleMean = 30;
		this->bendingAngleVariance = 5;
		this->rollAngleMean = 130;
		this->rollAngleVariance = 30;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.01f;

		this->numOfLateralBuds = 2;

		this->apicalLightFactor = 0.85f;
		this->lateralLightFactor = 0.2f;

		this->apicalDominanceBase = 4.87f;
		this->apicalDominanceDistance = 0.98f;
		this->apicalDominanceAge = 0.42f;
	}
	if (id == OAK) {
		this->initialGrowthRate = 3.0f;
		this->apicalControl = 2.2f;
		this->apicalControlAgeFactor = 0.5f;
		this->internodeBaseLength = 1.0f;
		this->internodeLengthAgeFactor = 0.93f;
		this->apicalAngleVariance = 20;

		this->bendingAngleMean = 29;
		this->bendingAngleVariance = 2;
		this->rollAngleMean = 91;
		this->rollAngleVariance = 1;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.21f;

		this->numOfLateralBuds = 3;

		this->apicalLightFactor = 0.39f;
		this->lateralLightFactor = 1.0f;

		this->apicalDominanceBase = 3.13f;
		this->apicalDominanceDistance = 0.13f;
		this->apicalDominanceAge = 0.82f;
	}
	if (id == PINE) {
		this->initialGrowthRate = 3.26f;
		this->apicalControl = 6.2f;
		this->apicalControlAgeFactor = 0.9f;
		this->internodeBaseLength = 0.4f;
		this->internodeLengthAgeFactor = 0.96f;
		this->apicalAngleVariance = 0;

		this->bendingAngleMean = 80;
		this->bendingAngleVariance = 4;
		this->rollAngleMean = 10;
		this->rollAngleVariance = 30;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.015f;

		this->numOfLateralBuds = 3;

		this->apicalLightFactor = 0.04f;
		this->lateralLightFactor = 0.1f;

		this->apicalDominanceBase = 0.01f;
		this->apicalDominanceDistance = 0.9f;
		this->apicalDominanceAge = 0.87f;
	}
}