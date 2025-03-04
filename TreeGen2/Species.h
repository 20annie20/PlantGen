// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM() 
enum Species_ID
{
	MAPLE,
	PINE,
	OAK
};

class TREEGEN2_API Species
{
public:

	Species_ID id = MAPLE;
	float initialGrowthRate;		// GR
	float apicalControl;			// AC
	float apicalControlAgeFactor;	// AC_AF
	float internodeBaseLength;		// IBL
	float internodeLengthAgeFactor;	// IL_AF
	int	  apicalAngleVariance;		// AAV

	int	  bendingAngleMean;		// BAM
	int	  bendingAngleVariance;	// BAV
	int	  rollAngleMean;			// RAM
	int	  rollAngleVariance;		// RAV
	
	float apicalExtinctionRate;		// AB_D
	float lateralExtinctionRate;	// LB_D
	int	  numOfLateralBuds;			// NLB
	float apicalLightFactor;		// LF_A
	float lateralLightFactor;		// LF_L
	float apicalDominanceBase;		// AD_BF
	float apicalDominanceDistance;	// AD_DF
	float apicalDominanceAge;		// AD_AF

	Species();
	Species(Species_ID);
	~Species();
};

