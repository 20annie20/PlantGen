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
    def add_branch(self, length):
        """ Interface method of adding a branch """

    @abstractmethod
    def finish(self):
        """ Interface method of finishing a skeleton """

    @abstractmethod
    def rotate(self, angle):
        """ Interface method of rotating a branch """


class TurtleSkeletonBuilder(SkeletonOutputBuilder):
    """ Skeleton drawer using turtle """
    drawStack = []

    def rotate(self, angle):
        """ rotate turtle """
        self.drawStack.append((right, angle))

    def add_branch(self, length):
        """ draw branch """
        self.drawStack.append((forward, length))

    def finish(self):
        """ execute all turtle commands """
        t = Turtle()
        map(lambda f, param: f(param), self.drawStack)
        t.screen.mainloop()


skeleton_builder = TurtleSkeletonBuilder()
