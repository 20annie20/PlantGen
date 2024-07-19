""" This module applies rules and executes them after all the replacements acceptable within bounding volume """

import random

from l_system_operator import Operator, Parser
from collections import deque


class Transformer:
    def __init__(self, operand_list: [Operator], rule_list: dict, iterations: int):
        self.stack = deque()
        self.stack.extend(operand_list)
        self.iterations = iterations
        self.rule_list = rule_list

    def replace(self):
        old_stack = self.stack.copy()
        self.stack.clear()
        parser = Parser()

        while self.iterations > 0:
            for operator in old_stack:
                if operator.symbol in self.rule_list.keys():
                    applicable_rules = self.rule_list[operator.symbol]
                    rule = random.choice(applicable_rules)
                    self.stack.extend(parser.mapListOfSymbols(rule))
                else:
                    self.stack.append(operator)
            self.iterations -= 1

    def transform(self):
        operator = self.stack.popleft()
        operator.execute()
