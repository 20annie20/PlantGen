# pragma once
#include <MeshCoordBuilder.h>

enum class EOperation
{
	YAW_RIGHT,
	YAW_LEFT,
	PITCH_DOWN,
	PITCH_UP,
	ROLL_LEFT,
	ROLL_RIGHT,
	TURN_AROUND,
	PUSH_STATE,
	POP_STATE
};


class Parser {
	TMap<EOperation, TCHAR> operator_characters = {
		{	EOperation::YAW_RIGHT,		'+'	},
		{	EOperation::YAW_LEFT,		'-'	},
		{	EOperation::PITCH_DOWN,		'&'	},
		{	EOperation::PITCH_UP,		'^'	},
		{	EOperation::ROLL_LEFT,		'\\'},
		{	EOperation::ROLL_RIGHT,		'/'	},
		{	EOperation::TURN_AROUND,	'|'	},
		{	EOperation::PUSH_STATE,		'['	},
		{	EOperation::POP_STATE,		']'	}
	};
	MeshBuilder* mesh_builder;
	TMap<TCHAR, TFunction<void()>> operator_sets;
public:
	Parser(float angle, float scale, MeshBuilder *mb);
	TFunction<void()> map_symbol(TCHAR symbol);
};