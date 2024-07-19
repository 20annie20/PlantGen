import json
import os

from Operator import *
from MeshOutputBuilder import *
from Transformer import Transformer
import argparse

input_parser = argparse.ArgumentParser()
input_parser.add_argument("-i", help="Initial generation ruleset")
input_parser.add_argument("-r", help="Initial generation ruleset")

if __name__ == "__main__":
    operator_parser = Parser()
    meshBuilder = TurtleMeshBuilder()

    args = input_parser.parse_args()
    instruction = args.i
    rules_file = args.r

    rules_list = json.load(open(os.path.normpath(rules_file)))

    operator_list = operator_parser.mapListOfSymbols(instruction)

    transformer = Transformer(operator_list, rules_list, 2)
    transformer.replace()
    while transformer.stack:
        transformer.transform(meshBuilder)

    meshBuilder.Finish()


