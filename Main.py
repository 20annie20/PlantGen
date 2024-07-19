from Rule import *
from MeshOutputBuilder import *
from Transformer import Transformer
import argparse

input_parser = argparse.ArgumentParser()
input_parser.add_argument("instr", help="Initial generation ruleset")

if __name__ == "__main__":
    rules_parser = Parser()
    meshBuilder = TurtleMeshBuilder()

    args = input_parser.parse_args()
    instruction = args.instr
    rules_list = []
    for char in instruction:
        rules_list.append(rules_parser.mapSymbol(char))

    transformer = Transformer(rules_list)
    while transformer.stack:
        transformer.transform(meshBuilder)

    meshBuilder.Finish()


