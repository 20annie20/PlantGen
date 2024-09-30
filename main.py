""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os

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

    if "ignore" in production_rules.keys():
        IGNORE_CHARACTERS = production_rules["ignore"]
        DO_IGNORE = True
    else:
        DO_IGNORE = False
        IGNORE_CHARACTERS = []

    meshBuilder = SkeletonBuilder(ANGLE, LENGTH, True)
    operator_parser = Parser3D(meshBuilder)

    transformer = LSystemSimulator(INITIAL_TREE,
                                   production_rules["rules"], ITERATIONS)
    transformer.init_parser(operator_parser)
    produced_word = transformer.produce(DO_IGNORE, IGNORE_CHARACTERS)

    operator_list = operator_parser.map_list_of_symbols(produced_word)

    transformer.apply_operators()

    meshBuilder.finish()
