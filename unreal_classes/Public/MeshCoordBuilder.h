// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
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
