""" This module contains different types of structure creators - 2D / 3D turtle debugging and tree file format
Skeleton is a representation of branches edges with orientations and lengths which can be further transformed
into spline meshes
"""

from abc import ABC, abstractmethod
from turtle import right, forward, Turtle


class SkeletonOutputBuilder(ABC):
    @abstractmethod
    def addBranch(self, length):
        pass

    @abstractmethod
    def finish(self):
        pass

    @abstractmethod
    def rotate(self, angle):
        pass


class TurtleSkeletonBuilder(SkeletonOutputBuilder):
    drawStack = []

    def rotate(self, angle):
        self.drawStack.append((right, angle))

    def addBranch(self, length):
        self.drawStack.append((forward, length))

    def finish(self):
        t = Turtle()
        [f(param) for f, param in self.drawStack]
        t.screen.mainloop()

skeleton_builder = TurtleSkeletonBuilder()