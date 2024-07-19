from MeshOutputBuilder import *

mesh = TurtleMeshBuilder()

class Rule(ABC):
    @abstractmethod
    def execute(self) -> bool:
        pass

class Branch(Rule):
    def execute(self) -> bool:
        return mesh.AddBranch(100)


class RotateRight(Rule):
    def execute(self) -> bool:
        return mesh.Rotate(90)

class RotateLeft(Rule):
    def execute(self) -> bool:
        return mesh.Rotate(-90)

class Parser:
    # TODO consider defining custom rules in init
    ruleSet = {
        "F": Branch(),
        "-": RotateLeft(),
        "+": RotateRight()
    }

    def mapSymbol(self, symbol: str):
        return self.ruleSet[symbol]

