""" This module contain operator definitions - L-system grammar """
from abc import ABC, abstractmethod

from skeleton_output_builder import skeleton_builder


class Operator(ABC):
    """ Interface abstract class Operator """
    @abstractmethod
    def execute(self) -> bool:
        pass


class Branch(Operator):
    """ Operator representing adding branch segment """
    symbol = "F"

    def execute(self) -> bool:
        return skeleton_builder.addBranch(10)


class RotateRight2D(Operator):
    """ Operator representing rotating the branch to the right in 2D """
    symbol = "+"

    def execute(self) -> bool:
        return skeleton_builder.rotate(90)


class RotateLeft(Operator):
    """ Operator representing rotating the branch to the left in 2D """
    symbol = "-"

    def execute(self) -> bool:
        return skeleton_builder.rotate(-90)


class Parser:
    """ Parses initial word strings and converts to list of rule objects """
    operatorSet = {
        "F": Branch(),
        "+": RotateRight2D(),
        "-": RotateLeft()
    }

    def mapSymbol(self, symbol: str) -> Operator:
        return self.operatorSet[symbol]

    def mapListOfSymbols(self, instruction: [str]) -> [Operator]:
        operator_list = []
        for char in instruction:
            operator_list.append(self.mapSymbol(char))
        return operator_list
