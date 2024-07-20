""" This module contains different types of structure creators
- 2D / 3D turtle debugging and tree file format
Skeleton is a representation of branches edges with orientations and lengths
which can be further transformed into spline meshes
"""
import turtle
from abc import ABC, abstractmethod
from turtle import Turtle  # pylint: disable = no-name-in-module


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

    @abstractmethod
    def push_state(self):
        """ Interface method of pushing current state onto a stack """

    @abstractmethod
    def pop_state(self):
        """ Interface method of popping the latest state from a stack """


class TurtleSkeletonBuilder(SkeletonOutputBuilder):
    """ Skeleton drawer using turtle """

    drawStack = []  # contains list of operations to execute
    stateStack = []  # contains states to draw with - (position, orientation)
    t = Turtle()

    def __init__(self):
        turtle.mode("logo") # pylint: disable = no-member
        self.t.speed(0)

    def rotate(self, angle):
        """ rotate turtle """
        self.t.right(angle)

    def add_branch(self, length):
        """ draw branch """
        self.t.forward(length)

    def push_state(self):
        """ add operation of adding turtle state to stack """
        self.get_turtle_state()

    def pop_state(self):
        """ add operation of popping & applying turtle state of the stack """
        self.set_turtle_state()

    def get_turtle_state(self):
        """ add current turtle state - location & rotation """
        self.stateStack.append([self.t.pos(), self.t.heading()])

    def set_turtle_state(self):
        """ pop latest turtle state - location & rotation """
        location, rotation = self.stateStack.pop()
        self.t.goto(location)
        self.t.seth(rotation)

    def finish(self):
        """ execute all turtle commands """
        self.t.screen.mainloop()


class PyGameSkeletonBuilder(SkeletonOutputBuilder):
    """ Takes the skeleton produced in 3D and renders it in pygame for debug """

    drawStack = []

    def rotate(self, angle):
        """ rotate  """

    def add_branch(self, length):
        """ draw """

    def finish(self):
        """ execute all commands """

    def push_state(self):
        """ push current state """

    def pop_state(self):
        """ pop latest state """


skeleton_builder = TurtleSkeletonBuilder()
