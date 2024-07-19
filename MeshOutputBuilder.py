from abc import ABC, abstractmethod
from turtle import *


class MeshOutputBuilder(ABC):
    @abstractmethod
    def AddBranch(self, length):
        pass

    @abstractmethod
    def Finish(self):
        pass

    @abstractmethod
    def Rotate(self, angle):
        pass


class TurtleMeshBuilder(MeshOutputBuilder):
    drawStack = []

    def Rotate(self, angle):
        self.drawStack.append((right, angle))

    def AddBranch(self, length):
        self.drawStack.append((forward, length))

    def Finish(self):
        t = Turtle()
        draw = [f(param) for f, param in self.drawStack]
        t.screen.mainloop()
