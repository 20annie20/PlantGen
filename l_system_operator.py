""" This module contain operator definitions - L-system grammar """
from abc import ABC, abstractmethod

from skeleton_output_builder import skeleton_builder


class Operator(ABC):
    """ Interface abstract class Operator """
    @abstractmethod
    def execute(self) -> bool:
        """ Interface method - execute operation """


class Branch(Operator):
    """ Operator representing adding branch segment """
    symbol = "F"

    def execute(self) -> bool:
        """ Add new branch segment """
        return skeleton_builder.addBranch(10)


class RotateRight2D(Operator):
    """ Operator representing rotating the branch to the right in 2D """
    symbol = "+"

    def execute(self) -> bool:
        """ Rotate branch to the right operation """
        return skeleton_builder.rotate(90)


class RotateLeft(Operator):
    """ Operator representing rotating the branch to the left in 2D """
    symbol = "-"

    def execute(self) -> bool:
        """ Rotate branch to the left operation """
        return skeleton_builder.rotate(-90)


class Parser:
    """ Parses initial word strings and converts to list of rule objects """
    operatorSet = {
        "F": Branch(),
        "+": RotateRight2D(),
        "-": RotateLeft()
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
