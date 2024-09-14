""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os
from pathlib import Path

from l_system_simulator.l_system_operator import Parser3D

from l_system_simulator.l_system import LSystemSimulator
from l_system_simulator.skeleton_output_builder import SkeletonBuilder
from unreal_scripts.generate_tree import generate_skeleton

if __name__ == "__main__":

    PRODUCTION_FILE = "rules/context_tree.json"

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

def main(load_from_file, file_path, initial_tree, rules_str, iterations, ignore_chars, angle, length, static_mesh_actor):
    os.chdir(Path(__file__).parents[0])

    if load_from_file:
        path = str(file_path)
        if os.path.isfile(path):
            with open(os.path.normpath(path), "r", encoding="utf-8") as file_handle:
                rules_content = json.load(file_handle)
                initial_tree = rules_content["axiom"]
                iterations = rules_content["iterations"]
                rules = rules_content["rules"]

                if "ignore" in rules_content.keys():
                    ignore_chars = rules_content["ignore"]
                    do_ignore = True
                else:
                    do_ignore = False
                    ignore_chars = []
        else:
            raise FileNotFoundError("Rules file not found.")
    else:
        initial_tree = str(initial_tree)
        iterations = int(iterations)

        ignore_chars = str(ignore_chars)
        if len(ignore_chars) > 0:
            do_ignore = True
        else:
            do_ignore = False
        rules = json.loads(str(rules_str))
    word = LSystemSimulator.produce_word(do_ignore, ignore_chars, initial_tree, rules, iterations)
    generate_skeleton(static_mesh_actor, word, angle, length)