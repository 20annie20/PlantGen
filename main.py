""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os
# import argparse

from l_system_simulator.l_system_operator import Parser3D

from l_system_simulator.l_system import LSystemSimulator
from l_system_simulator.skeleton_output_builder import SkeletonBuilder


if __name__ == "__main__":

    PRODUCTION_FILE = "rules/3d_tree1.json"

    with open(os.path.normpath(PRODUCTION_FILE), "r", encoding="utf-8") as file_handle:
        production_rules = json.load(file_handle)

    INITIAL_TREE = production_rules["axiom"]
    ITERATIONS = production_rules["iterations"]
    ANGLE = production_rules["angle"]
    LENGTH = production_rules["length"]

    meshBuilder = SkeletonBuilder(ANGLE, LENGTH, True)
    operator_parser = Parser3D(meshBuilder)

    transformer = LSystemSimulator(operator_parser, INITIAL_TREE,
                                   production_rules["rules"], ITERATIONS)
    produced_word = transformer.produce()

    operator_list = operator_parser.map_list_of_symbols(produced_word)

    transformer.apply_operators()

    meshBuilder.finish()
