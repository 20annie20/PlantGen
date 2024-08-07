""" This module contain operator mappings - L-system grammar """
from enum import Enum


class OperatorCharacters(Enum):
    """ Mapping characters to operations """
    YAW_RIGHT = "+"
    YAW_LEFT = "-"
    PITCH_DOWN = "&"
    PITCH_UP = "^"
    ROLL_LEFT = "\\"
    ROLL_RIGHT = "/"
    TURN_AROUND = "|"
    PUSH_STATE = "["
    POP_STATE = "]"


class Parser3D:
    """ Matches functions for each operator in the grammar word. """

    def __init__(self, skeleton_builder):
        self.operator_set = {
            OperatorCharacters.YAW_RIGHT.value: skeleton_builder.yaw_right,
            OperatorCharacters.YAW_LEFT.value: skeleton_builder.yaw_left,
            OperatorCharacters.PITCH_DOWN.value: skeleton_builder.pitch_down,
            OperatorCharacters.PITCH_UP.value: skeleton_builder.pitch_up,
            OperatorCharacters.ROLL_LEFT.value: skeleton_builder.roll_left,
            OperatorCharacters.ROLL_RIGHT.value: skeleton_builder.roll_right,
            OperatorCharacters.TURN_AROUND.value: skeleton_builder.turn_around,
            OperatorCharacters.PUSH_STATE.value: skeleton_builder.push_state,
            OperatorCharacters.POP_STATE.value: skeleton_builder.pop_state
        }
        for i in range(ord("A"), ord("Z")):
            self.operator_set[chr(i)] = skeleton_builder.add_branch

        for i in range(ord("0"), ord("9")):
            self.operator_set[chr(i)] = skeleton_builder.idle

    def map_symbol(self, symbol: str):
        """ Map character to a rule class """
        return self.operator_set[symbol]

    def map_list_of_symbols(self, instruction: [str]):
        """ Map chars in a word instruction to a list of rules """
        operator_list = []
        for char in instruction:
            operator_list.append(self.map_symbol(char))
        return operator_list
