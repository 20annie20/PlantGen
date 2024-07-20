""" This module contain operator definitions - L-system grammar """
from abc import ABC, abstractmethod
from dataclasses import dataclass

from l_system_simulator.skeleton_output_builder import skeleton_builder

ANGLE = 25.7
LENGTH = 3


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
class RotateLeft(Operator):
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


class Parser:
    """ Parses initial word strings and converts to list of rule objects """
    operatorSet = {
        "F": Branch(),
        "X": Branch(),
        "+": RotateRight2D(),
        "-": RotateLeft(),
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
