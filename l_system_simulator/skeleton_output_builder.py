""" This module contains different types of structure creators
- 2D / 3D turtle debugging and tree file format
Skeleton is a representation of branches edges with orientations and lengths
which can be further transformed into spline meshes
"""

import turtle
from turtle import Turtle  # pylint: disable = no-name-in-module

import pygame  # pylint: disable = import-error
import numpy as np


class TurtleSkeletonBuilder:
    """ Skeleton drawer using turtle """

    stateStack = []  # contains states to draw with - (position, orientation)

    def __init__(self, angle, length):
        self.t = Turtle()
        turtle.mode("logo")  # pylint: disable = no-member
        self.t.speed(0)
        self.angle = angle
        self.length = length

    def rotate(self, vector):
        """ rotate turtle """
        self.t.right(self.angle)

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


def rotate(vector, angle):
    c, s = np.cos(angle), np.sin(angle)
    x, y, z = vector
    r = np.array([
        [c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s],
        [y * x * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s],
        [z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, c + z * z * (1 - c)]
    ])
    return r


class PyGameSkeletonBuilder:
    """ Takes the skeleton produced in 3D and renders it in pygame for debug """
    SCREEN_SIZE_X = 860
    SCREEN_SIZE_Y = 640
    stateStack = []  # stores stack of positions & rotations
    pygame.init()  # pylint: disable = no-member

    def __init__(self, angle, length, orient_y=False):
        self.curr_pos = np.array([0.0, 0.0, 0.0])
        self.heading = np.array([1.0, 0.0, 0.0])
        self.left = np.array([0.0, 1.0, 0.0])
        self.up = np.array([0.0, 0.0, -1.0])
        self.angle = np.radians(angle)
        self.length = length
        if orient_y:
            r = rotate(self.up, -np.radians(90))
            self.heading = r @ self.heading
            self.left = r @ self.left
            self.curr_pos += np.array([0.0, -60.0, 0.0])

    # Set up display
    screen = pygame.display.set_mode((SCREEN_SIZE_X, SCREEN_SIZE_Y))
    screen.fill("white")
    pygame.display.set_caption('PlantGen debug window')

    def yaw_left(self):
        r = rotate(self.up, self.angle)
        self.heading = r @ self.heading
        self.left = r @ self.left

    def yaw_right(self):
        r = rotate(self.up, -self.angle)
        self.heading = r @ self.heading
        self.left = r @ self.left

    def turn_around(self):
        r = rotate(self.up, np.radians(180))
        self.heading = r @ self.heading
        self.left = r @ self.left

    def pitch_down(self):
        r = rotate(self.left, self.angle)
        self.heading = r @ self.heading
        self.up = r @ self.up

    def pitch_up(self):
        r = rotate(self.left, -self.angle)
        self.heading = r @ self.heading
        self.up = r @ self.up

    def roll_left(self):
        r = rotate(self.heading, self.angle)
        self.left = r @ self.left
        self.up = r @ self.up

    def roll_right(self):
        r = rotate(self.heading, -self.angle)
        self.left = r @ self.left
        self.up = r @ self.up

    def add_branch(self):
        """ draw branch """
        end_pos = self.get_end_pos(self.length)
        start, end = self.perspective_transform_line(end_pos)
        # perspective to 2D
        pygame.draw.line(
            self.screen,
            color=pygame.Color("black"),
            start_pos=start,
            end_pos=end,
            width=1
        )
        self.curr_pos = end_pos

    def get_end_pos(self, length) -> ():
        return self.curr_pos + self.heading * length

    def perspective_transform_line(self, end_pos) -> ((int, int), (int, int)):
        start = self.perspective_transform_point(self.curr_pos)
        end = self.perspective_transform_point(end_pos)
        return start, end

    def perspective_transform_point(self, point) -> (int, int):
        x = point[0]
        y = point[1]
        z = point[2] - 4

        out_x = self.SCREEN_SIZE_X / 2 + z * x
        out_y = self.SCREEN_SIZE_Y / 2 - (z * -y)
        return int(out_x), int(out_y)

    @staticmethod
    def finish():
        """ setup main loop """
        running = True
        pygame.display.flip()
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:  # pylint: disable = no-member
                    running = False

    def push_state(self):
        """ add current state - location & rotation """
        self.stateStack.append([self.curr_pos, self.heading, self.up, self.left])

    def pop_state(self):
        """ pop latest turtle state - location & rotation """
        location, heading, up, left = self.stateStack.pop()
        self.curr_pos = location
        self.heading = heading
        self.up = up
        self.left = left
