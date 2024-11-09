#include "Parser.h"
#include "functional"

Parser::Parser(float angle, float scale, MeshBuilder *mb)
{
	mesh_builder = mb;

	operator_sets = {
		{operator_characters[EOperation::YAW_RIGHT], std::bind(&MeshBuilder::yaw_right, mesh_builder)},
		{operator_characters[EOperation::YAW_LEFT], std::bind(&MeshBuilder::yaw_left, mesh_builder)},
		{operator_characters[EOperation::PITCH_DOWN], std::bind(&MeshBuilder::pitch_down, mesh_builder)},
		{operator_characters[EOperation::PITCH_UP], std::bind(&MeshBuilder::pitch_up, mesh_builder)},
		{operator_characters[EOperation::ROLL_LEFT], std::bind(&MeshBuilder::roll_left, mesh_builder)},
		{operator_characters[EOperation::ROLL_RIGHT], std::bind(&MeshBuilder::roll_right, mesh_builder)},
		{operator_characters[EOperation::TURN_AROUND], std::bind(&MeshBuilder::turn_around, mesh_builder)},
		{operator_characters[EOperation::PUSH_STATE], std::bind(&MeshBuilder::push_state, mesh_builder)},
		{operator_characters[EOperation::POP_STATE], std::bind(&MeshBuilder::pop_state, mesh_builder)},
	};


	for (int i = static_cast<int>('A'); i < static_cast<int>('Z'); ++i) {
		operator_sets.Add({static_cast<TCHAR>(i), std::bind(&MeshBuilder::add_branch, mesh_builder)});
	}

	for (int i = static_cast<int>('0'); i < static_cast<int>('9'); ++i) {
		operator_sets.Add({static_cast<TCHAR>(i), std::bind(&MeshBuilder::idle, mesh_builder)});
	}
	
}

TFunction<void()> Parser::map_symbol(TCHAR symbol)
{
	return operator_sets[symbol];
}
