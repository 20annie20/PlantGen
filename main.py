""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os
import argparse

from l_system_operator import Parser
from skeleton_output_builder import TurtleSkeletonBuilder
from transformer import Transformer

input_parser = argparse.ArgumentParser()
input_parser.add_argument("-i", help="Initial generation ruleset")
input_parser.add_argument("-r", help="Initial generation ruleset")

if __name__ == "__main__":
    operator_parser = Parser()
    meshBuilder = TurtleSkeletonBuilder()

    args = input_parser.parse_args()
    instruction = args.i
    rules_file = args.r

    with open(os.path.normpath(rules_file), "r", encoding="utf-8") as file_handle:
        rules_list = json.load(file_handle)

    operator_list = operator_parser.mapListOfSymbols(instruction)

    transformer = Transformer(operator_list, rules_list, 2)
    transformer.replace()
    while transformer.stack:
        transformer.transform()

    meshBuilder.finish()
