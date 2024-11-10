#pragma once

#include "CoreMinimal.h"
#include "Tree.h"

using namespace UE::Math;

struct Turtle
{
	FVector current_position;
	FVector heading;
	FVector up;
	FVector left;
};

class TREEGENPLUGIN_API MeshBuilder
{
protected:
	float length;
	float angle;
	Turtle curr_state;
	TArray<Turtle> state_stack;
	TArray<FVector> points;

public:
	MeshBuilder();
	MeshBuilder(float angle, float scale);
	FVector get_curr_pos();
	void yaw_right();
	void yaw_left();
	void pitch_down();
	void pitch_up();
	void roll_left();
	void roll_right();
	void turn_around();
	void push_state();
	void pop_state();
	void add_branch();
	void idle();
};

class ParametricMeshBuilder : public MeshBuilder 
{
protected:
	SpeciesParams speciesParams;
public:
	ParametricMeshBuilder(SpeciesParams speciesParams);
	void CalcBranch(Branch& branch);
};
