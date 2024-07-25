import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


# Define the rotation matrices
def rotate_around_vector(vector, angle):
    c, s = np.cos(angle), np.sin(angle)
    x, y, z = vector
    R = np.array([
        [c + x * x * (1 - c), x * y * (1 - c) + z * s, x * z * (1 - c) - y * s],
        [y * x * (1 - c) - z * s, c + y * y * (1 - c), y * z * (1 - c) + x * s],
        [z * x * (1 - c) + y * s, z * y * (1 - c) - x * s, c + z * z * (1 - c)]
    ])
    return R


class Turtle3D:
    def __init__(self):
        self.position = np.array([0.0, 0.0, 0.0])
        self.heading = np.array([1.0, 0.0, 0.0])
        self.left = np.array([0.0, 1.0, 0.0])
        self.up = np.array([0.0, 0.0, -1.0])
        self.positions = [self.position.copy()]

    def forward(self, distance):
        assert np.linalg.norm(self.heading) == 1.0
        self.position += self.heading * distance
        self.positions.append(self.position.copy())

    def pitch_down(self, angle):
        R = rotate_around_vector(self.left, angle)
        self.heading = R @ self.heading
        self.up = R @ self.up

    def pitch_up(self, angle):
        self.pitch_down(-angle)

    def roll_left(self, angle):
        R = rotate_around_vector(self.heading, angle)
        self.left = R @ self.left
        self.up = R @ self.up

    def roll_right(self, angle):
        self.roll_left(-angle)

    def yaw_left(self, angle):
        R = rotate_around_vector(self.up, angle)
        self.heading = R @ self.heading
        self.left = R @ self.left

    def yaw_right(self, angle):
        self.yaw_left(-angle)

    def turn_around(self):
        R = rotate_around_vector(self.up, np.radians(180))
        self.heading = R @ self.heading
        self.left = R @ self.left


def interpret_L_system(axiom, rules, iterations, distance, angle):
    system = axiom
    for _ in range(iterations):
        system = ''.join(rules.get(c, c) for c in system)

    turtle = Turtle3D()
    angle_rad = np.radians(angle)
    for command in system:
        if ord(command) in list(range(ord("A"), ord("Z"))):
            turtle.forward(distance)
        elif command == '+':
            turtle.yaw_right(angle_rad)
        elif command == '-':
            turtle.yaw_left(angle_rad)
        elif command == '&':
            turtle.pitch_down(angle_rad)
        elif command == '^':
            turtle.pitch_up(angle_rad)
        elif command == '\\':
            turtle.roll_left(angle_rad)
        elif command == '/':
            turtle.roll_right(angle_rad)
        elif command == '|':
            turtle.turn_around()

    return turtle.positions


# Example usage

axiom = "A"
rules = {
    "A": "B-F+CFC+F-D&F^D-F+&&CFC+F+B//",
    "B": "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//",
    "C": "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//",
    "D": "|CFB-F+B|FA&F^A&&FB-F+B|FC//"
}

iterations = 2
distance = 1
angle = 90

positions = interpret_L_system(axiom, rules, iterations, distance, angle)

# Plot the result
fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")
positions = np.array(positions)
ax.plot(positions[:, 0], positions[:, 1], positions[:, 2])
plt.show()
