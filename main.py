""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os
# import argparse

from l_system_simulator.l_system_operator import Parser

from l_system_simulator.l_system import LSystemSimulator
from l_system_simulator.skeleton_output_builder import TurtleSkeletonBuilder

# input_parser = argparse.ArgumentParser()
# input_parser.add_argument("-i", help="Initial tree")
# input_parser.add_argument("-p", help="Production rules")

if __name__ == "__main__":
    operator_parser = Parser()
    meshBuilder = TurtleSkeletonBuilder()

    # args = input_parser.parse_args()
    # initial_tree = args.i
    # production_file = args.r
    INITIAL_TREE = "X"
    PRODUCTION_FILE = "rules/rules.json"
    ITERATIONS = 5

    with open(os.path.normpath(PRODUCTION_FILE), "r", encoding="utf-8") as file_handle:
        production_rules = json.load(file_handle)

    transformer = LSystemSimulator(INITIAL_TREE, production_rules, ITERATIONS)
    produced_word = transformer.produce()

    operator_list = operator_parser.map_list_of_symbols(produced_word)

    transformer.apply_operators()

    meshBuilder.finish()
