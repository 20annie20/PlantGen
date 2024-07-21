""" This module contain operator definitions - L-system grammar """
import math
from abc import ABC, abstractmethod
from dataclasses import dataclass

from l_system_simulator.skeleton_output_builder import skeleton_builder

ANGLE = math.radians(87)
LENGTH = 2


@dataclass
class Operator(ABC):
    """ Interface abstract class Operator """

    @abstractmethod
    def execute(self):
        """ Interface method - execute operation """


@dataclass
class Branch(Operator):
    """ Operator representing adding branch segment """
    symbol = "F"

    def execute(self):
        """ Add new branch segment """
        skeleton_builder.add_branch(LENGTH)


@dataclass
class RotateRight2D(Operator):
    """ Operator representing rotating the branch to the right in 2D """
    symbol = "+"

    def execute(self):
        """ Rotate branch to the right operation """
        skeleton_builder.rotate(-ANGLE)


@dataclass
class RotateLeft2D(Operator):
    """ Operator representing rotating the branch to the left in 2D """
    symbol = "-"

    def execute(self):
        """ Rotate branch to the left operation """
        skeleton_builder.rotate(ANGLE)


@dataclass
class PushStack(Operator):
    """ Operator representing pushing current state onto a stack """
    symbol = "["

    def execute(self):
        """ Push current state onto a skeleton's stack """
        skeleton_builder.push_state()


@dataclass
class PopStack(Operator):
    """ Operator representing popping current state from a stack """
    symbol = "]"

    def execute(self):
        """ Push latest state from a skeleton's stack """
        skeleton_builder.pop_state()


class YawLeft(Operator):
    """ Operator representing yawing to the left"""
    symbol = "-"

    def execute(self):
        """ Apply rotation matrix """
        skeleton_builder.yaw_left(ANGLE)


class YawRight(Operator):
    """ Operator representing yawing to the right """
    symbol = "+"

    def execute(self):
        """ Apply rotation matrix """
        skeleton_builder.yaw_right(ANGLE)


class PitchDown(Operator):
    """ Operator representing pitch down """
    symbol = "&"

    def execute(self):
        """ Apply rotation matrix """
        skeleton_builder.pitch_down(ANGLE)


class PitchUp(Operator):
    """ Operator representing pitch up """
    symbol = "^"

    def execute(self):
        """ Apply rotation matrix """
        skeleton_builder.pitch_up(ANGLE)


class RollLeft(Operator):
    """ Operator representing roll left """
    symbol = "\\"

    def execute(self):
        """ Apply rotation matrix """
        skeleton_builder.roll_left(ANGLE)


class RollRight(Operator):
    """ Operator representing roll right """
    symbol = "/"

    def execute(self):
        """ Apply rotation matrix"""
        skeleton_builder.roll_right(ANGLE)


class Parser:
    """ Parses initial word strings and converts to list of 2D rule objects """
    operatorSet = {
        "F": Branch(),
        "X": Branch(),
        "+": RotateRight2D(),
        "-": RotateLeft2D(),
        "[": PushStack(),
        "]": PopStack()
    }

    def map_symbol(self, symbol: str) -> Operator:
        """ Map character to a rule class """
        return self.operatorSet[symbol]

    def map_list_of_symbols(self, instruction: [str]) -> [Operator]:
        """ Map chars in a word instruction to a list of rules """
        operator_list = []
        for char in instruction:
            operator_list.append(self.map_symbol(char))
        return operator_list


class Parser3D(Parser):
    """ Extends 2D parser for three-dimensional operations """
    operatorSet = {
        "F": Branch(),
        "X": Branch(),
        "A": Branch(),
        "B": Branch(),
        "C": Branch(),
        "D": Branch(),
        "+": YawLeft(),
        "-": YawRight(),
        "&": PitchDown(),
        "^": PitchUp(),
        "\\": RollLeft(),
        "/": RollRight(),
        #"|": TurnAround(),
        "[": PushStack(),
        "]": PopStack()
    }
