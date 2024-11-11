// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshCoordBuilder.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include <random>

MeshBuilder::MeshBuilder()
{
	this->length = 100.0;
	this->angle = UKismetMathLibrary::DegreesToRadians(0.0);
	this->curr_state.current_position = FVector(0.0f);
	this->curr_state.heading = FVector(0.0f);
	this->curr_state.up = FVector(0.0f);
	this->curr_state.left = FVector(0.0f);
}

MeshBuilder::MeshBuilder(float angle, float scale)
{
	this->length = 100.0 * scale;
	this->angle = UKismetMathLibrary::DegreesToRadians(angle);
	this->curr_state.current_position = FVector(0.0f);
	this->curr_state.heading = FVector(0.0f, 0.0f, 1.0f);
	this->curr_state.up = FVector(0.0f, -1.0f, 0.0f);
	this->curr_state.left = FVector(1.0f, 0.0f, 0.0f);
}

FVector MeshBuilder::get_curr_pos() {
	return this->curr_state.current_position;
}

void MeshBuilder::roll_right()
{
	Turtle state = this->curr_state;
	this->curr_state.left = state.left.RotateAngleAxisRad(-this->angle, state.heading);
	this->curr_state.up = state.up.RotateAngleAxisRad(-this->angle, state.heading);
}

void MeshBuilder::roll_left()
{
	Turtle state = this->curr_state;
	this->curr_state.left = state.left.RotateAngleAxisRad(this->angle, state.heading);
	this->curr_state.up = state.up.RotateAngleAxisRad(this->angle, state.heading);
}

void MeshBuilder::pitch_down()
{
	Turtle state = this->curr_state;
	this->curr_state.heading = state.heading.RotateAngleAxisRad(this->angle, state.left);
	this->curr_state.up = state.up.RotateAngleAxisRad(this->angle, state.left);
}

void MeshBuilder::pitch_up()
{
	Turtle state = this->curr_state;
	this->curr_state.heading = state.heading.RotateAngleAxisRad(-this->angle, state.left);
	this->curr_state.up = state.up.RotateAngleAxisRad(-this->angle, state.left);
}

void MeshBuilder::yaw_left()
{
	Turtle state = this->curr_state;
	this->curr_state.heading = state.heading.RotateAngleAxisRad(this->angle, state.up);
	this->curr_state.left = state.left.RotateAngleAxisRad(this->angle, state.up);
}

void MeshBuilder::yaw_right()
{
	Turtle state = this->curr_state;
	this->curr_state.heading = state.heading.RotateAngleAxisRad(-this->angle, state.up);
	this->curr_state.left = state.left.RotateAngleAxisRad(-this->angle, state.up);
}

void MeshBuilder::turn_around()
{
	Turtle state = this->curr_state;
	double turn_angle = UKismetMathLibrary::DegreesToRadians(180);
	this->curr_state.heading = state.heading.RotateAngleAxisRad(this->angle, state.up);
	this->curr_state.left = state.left.RotateAngleAxisRad(this->angle, state.up);
}

void MeshBuilder::push_state()
{
	this->state_stack.Add(this->curr_state);
}

void MeshBuilder::pop_state()
{
	this->points.Add(this->get_curr_pos());
	this->curr_state = this->state_stack.Pop();
}

void MeshBuilder::add_branch()
{
	auto end_pos = this->curr_state.current_position + this->curr_state.heading * this->length;
	this->points.Add(end_pos);
	this->curr_state.current_position = end_pos;
}

void MeshBuilder::idle()
{
}

ParametricMeshBuilder::ParametricMeshBuilder(SpeciesParams speciesParams, int tree_age) {
	this->speciesParams = speciesParams;
	this->length = speciesParams.ILB * 10.0f;
	this->curr_state.current_position = FVector(0.0f);
	this->curr_state.heading = FVector(0.0f, 0.0f, 1.0f);
	this->curr_state.up = FVector(0.0f, -1.0f, 0.0f);
	this->curr_state.left = FVector(1.0f, 0.0f, 0.0f);
	this->tree_age = tree_age;
}

void ParametricMeshBuilder::CalcBranch(Branch& branch, Branch& parent) {
	std::random_device rd{};
	std::mt19937 gen{ rd() };

	if (branch.parent != NULL) {
		this->curr_state.current_position = parent.nodes[branch.parent_node_idx].coordinates;
		this->curr_state.heading = parent.nodes[branch.parent_node_idx].heading;
		// modify heading by adding species lateral bud rotation and bend

		std::normal_distribution bd{ (float)speciesParams.BAM, (float)speciesParams.BAV }; // bend
		std::normal_distribution rotd{ (float)speciesParams.RAM, (float)speciesParams.RAV }; // rotate
		this->angle = bd(gen);
		this->roll_left();

	}
	else {
		this->curr_state.current_position = FVector(0.0);
		this->curr_state.heading = FVector(0.0, 0.0, 1.0);
	}

	for (int idx = 0; idx < branch.nodes.Num(); idx++) {
		auto end_pos = this->curr_state.current_position + this->curr_state.heading * this->length;
		branch.nodes[idx].coordinates = curr_state.current_position;
		branch.nodes[idx].heading = curr_state.heading;

		// apical angle variance
		FVector2D polar_heading;
		
		std::normal_distribution d{(float)speciesParams.AAV};

		polar_heading.X = d(gen);
		polar_heading.Y = FMath::RandRange(0.0, 2*3.14);
		this->curr_state.current_position = end_pos;
		auto var = polar_heading.SphericalToUnitCartesian();
		this->curr_state.heading = this->curr_state.heading + 0.3 * var;
		this->curr_state.heading.Normalize();
	}
}
