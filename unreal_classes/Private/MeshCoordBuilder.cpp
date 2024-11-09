// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshCoordBuilder.h"
#include "Kismet/KismetMathLibrary.h"

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
