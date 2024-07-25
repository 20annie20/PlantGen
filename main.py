""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os
import numpy as np
# import argparse

from l_system_simulator.l_system_operator import Parser3D

from l_system_simulator.l_system import LSystemSimulator
from l_system_simulator.skeleton_output_builder import PyGameSkeletonBuilder

# input_parser = argparse.ArgumentParser()
# input_parser.add_argument("-i", help="Initial tree")
# input_parser.add_argument("-p", help="Production rules")

if __name__ == "__main__":
    # args = input_parser.parse_args()
    # initial_tree = args.i
    # production_file = args.r

    INITIAL_TREE = "A"
    PRODUCTION_FILE = "rules/rules.json"
    ITERATIONS = 2
    ANGLE = np.radians(90)
    LENGTH = 2

    meshBuilder = PyGameSkeletonBuilder(ANGLE, LENGTH)
    operator_parser = Parser3D(meshBuilder)

    with open(os.path.normpath(PRODUCTION_FILE), "r", encoding="utf-8") as file_handle:
        production_rules = json.load(file_handle)

    transformer = LSystemSimulator(operator_parser, INITIAL_TREE, production_rules, ITERATIONS)
    produced_word = transformer.produce()
    print(produced_word)

    operator_list = operator_parser.map_list_of_symbols(produced_word)

    transformer.apply_operators()

    meshBuilder.finish()
