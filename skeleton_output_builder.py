""" This module contains different types of structure creators
- 2D / 3D turtle debugging and tree file format
Skeleton is a representation of branches edges with orientations and lengths
which can be further transformed into spline meshes
"""

from abc import ABC, abstractmethod
from turtle import right, forward, Turtle


class SkeletonOutputBuilder(ABC):
    """ Interface class of a skeleton builder """
    @abstractmethod
    def addBranch(self, length):
        """ Interface method of adding a branch """
        pass

    @abstractmethod
    def finish(self):
        """ Interface method of finishing a skeleton """
        pass

    @abstractmethod
    def rotate(self, angle):
        """ Interface method of rotating a branch """
        pass


class TurtleSkeletonBuilder(SkeletonOutputBuilder):
    """ Skeleton drawer using turtle """
    drawStack = []

    def rotate(self, angle):
        """"""
        self.drawStack.append((right, angle))

    def addBranch(self, length):
        """"""
        self.drawStack.append((forward, length))

    def finish(self):
        """"""
        t = Turtle()
        map(lambda f, param: f(param), self.drawStack)
        t.screen.mainloop()


skeleton_builder = TurtleSkeletonBuilder()
