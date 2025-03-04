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
		this->apicalAngleVariance = 38;

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
}