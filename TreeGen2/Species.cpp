// Fill out your copyright notice in the Description page of Project Settings.


#include "Species.h"

Species::Species() {

};

Species::~Species() {

};

Species::Species(Species_ID newId)
{
	this->id = newId;
	if (id == MAPLE) {
		this->initialGrowthRate = 0.98f;
		this->apicalControl = 2.4f;
		this->apicalControlAgeFactor = 0.85f;
		this->internodeBaseLength = 1.02f;
		this->internodeLengthAgeFactor = 0.97f;
		this->apicalAngleVariance = 20;

		this->bendingAngleMean = 38;
		this->bendingAngleVariance = 2;
		this->rollAngleMean = 91;
		this->rollAngleVariance = 1;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.21f;
		this->numOfLateralBuds = 4;
		this->apicalLightFactor = 0.39;
		this->lateralLightFactor = 1.13;
		this->apicalDominanceBase = 3.13f;
		this->apicalDominanceDistance = 0.13f;
		this->apicalDominanceAge = 0.82f;
	}
	if (id == PINE) {
		this->initialGrowthRate = 1.9f;
		this->apicalControl = 0.27f;
		this->apicalControlAgeFactor = 0.9f;
		this->internodeBaseLength = 0.49f;
		this->internodeLengthAgeFactor = 0.98f;
		this->apicalAngleVariance = 1;

		this->bendingAngleMean = 41;
		this->bendingAngleVariance = 3;
		this->rollAngleMean = 87;
		this->rollAngleVariance = 2;

		this->apicalExtinctionRate = 0.0f;
		this->lateralExtinctionRate = 0.21f;
		this->numOfLateralBuds = 2;
		this->apicalLightFactor = 0.37f;
		this->lateralLightFactor = 1.05f;
		this->apicalDominanceBase = 0.37f;
		this->apicalDominanceDistance = 0.31;
		this->apicalDominanceAge = 0.9f;
	}
}