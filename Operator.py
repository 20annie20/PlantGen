""" This module contain operator definitions - L-system grammar """

from SkeletonOutputBuilder import *

mesh = TurtleSkeletonBuilder()


class Operator(ABC):
    @abstractmethod
    def execute(self) -> bool:
        pass


class Branch(Operator):
    symbol = "F"

    def execute(self) -> bool:
        return mesh.addBranch(10)


class RotateRight(Operator):
    symbol = "+"

    def execute(self) -> bool:
        return mesh.rotate(90)


class RotateLeft(Operator):
    symbol = "-"

    def execute(self) -> bool:
        return mesh.rotate(-90)


class Parser:
    operatorSet = {
        "F": Branch(),
        "+": RotateRight(),
        "-": RotateLeft()
    }

    def mapSymbol(self, symbol: str) -> Operator:
        return self.operatorSet[symbol]

    def mapListOfSymbols(self, instruction: [str]) -> [Operator]:
        operator_list = []
        for char in instruction:
            operator_list.append(self.mapSymbol(char))
        return operator_list
