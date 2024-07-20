""" This module contains different types of structure creators
- 2D / 3D turtle debugging and tree file format
Skeleton is a representation of branches edges with orientations and lengths
which can be further transformed into spline meshes
"""
import math
import turtle
from abc import ABC, abstractmethod
from turtle import Turtle  # pylint: disable = no-name-in-module

import pygame


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

    stateStack = []  # contains states to draw with - (position, orientation)

    def __init__(self):
        self.t = Turtle()
        turtle.mode("logo")  # pylint: disable = no-member
        self.t.speed(0)

    def rotate(self, angle):
        """ rotate turtle """
        self.t.right(angle)

    def add_branch(self, length):
        """ draw branch """
        self.t.forward(length)

    def push_state(self):
        """ add current turtle state - location & rotation """
        self.stateStack.append([self.t.pos(), self.t.heading()])

    def pop_state(self):
        """ pop latest turtle state - location & rotation """
        location, rotation = self.stateStack.pop()
        self.t.goto(location)
        self.t.seth(rotation)

    def finish(self):
        """ execute all turtle commands """
        self.t.screen.mainloop()


class PyGameSkeletonBuilder(SkeletonOutputBuilder):
    """ Takes the skeleton produced in 3D and renders it in pygame for debug """
    SCREEN_SIZE_X = 860
    SCREEN_SIZE_Y = 640
    stateStack = []  # stores stack of positions & rotations
    curr_pos = (SCREEN_SIZE_X/2, SCREEN_SIZE_Y)
    curr_rot = -90
    pygame.init()

    # Set up display
    screen = pygame.display.set_mode((SCREEN_SIZE_X, SCREEN_SIZE_Y))
    screen.fill("white")
    pygame.display.set_caption('PlantGen debug window')

    def rotate(self, angle):
        """ Change current rotation """
        self.curr_rot += angle

    def add_branch(self, length):
        """ draw branch """
        end_pos = self.get_end_pos(length)
        pygame.draw.line(
            self.screen,
            color=pygame.Color("black"),
            start_pos=self.curr_pos,
            end_pos=end_pos,
            width=1
        )
        self.curr_pos = end_pos

    def get_end_pos(self, length):
        """ Calculate the end of the line """
        angle_radians = math.radians(self.curr_rot)

        delta_x = length * math.cos(angle_radians)
        delta_y = length * math.sin(angle_radians)

        end_x = self.curr_pos[0] + delta_x
        end_y = self.curr_pos[1] + delta_y

        return end_x, end_y

    def finish(self):
        """ setup main loop """
        running = True
        pygame.display.flip()
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False

    def push_state(self):
        """ add current state - location & rotation """
        self.stateStack.append([self.curr_pos, self.curr_rot])

    def pop_state(self):
        """ pop latest turtle state - location & rotation """
        location, rotation = self.stateStack.pop()
        self.curr_pos = location
        self.curr_rot = rotation


skeleton_builder = PyGameSkeletonBuilder()
