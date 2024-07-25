""" This module contain operator mappings - L-system grammar """


class Parser3D:
    def __init__(self, skeleton_builder):
        skeleton_builder = skeleton_builder
        """ Extends 2D parser for three-dimensional operations """
        self.operatorSet = {
            "+": skeleton_builder.yaw_right,
            "-": skeleton_builder.yaw_left,
            "&": skeleton_builder.pitch_down,
            "^": skeleton_builder.pitch_up,
            "\\": skeleton_builder.roll_left,
            "/": skeleton_builder.roll_right,
            "|": skeleton_builder.turn_around,
            "[": skeleton_builder.push_state,
            "]": skeleton_builder.pop_state
        }
        for i in range(ord("A"), ord("Z")):
            self.operatorSet[chr(i)] = skeleton_builder.add_branch

    def map_symbol(self, symbol: str):
        """ Map character to a rule class """
        return self.operatorSet[symbol]

    def map_list_of_symbols(self, instruction: [str]):
        """ Map chars in a word instruction to a list of rules """
        operator_list = []
        for char in instruction:
            operator_list.append(self.map_symbol(char))
        return operator_list
