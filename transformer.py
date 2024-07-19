""" This module applies rules and executes them
after all the replacements acceptable within bounding volume
"""

import random
from collections import deque

from l_system_operator import Operator, Parser


class Transformer:
    """ Apply grammar rules and apply operators """
    def __init__(self, operand_list: [Operator], rule_list: dict, iterations: int):
        self.stack = deque()
        self.stack.extend(operand_list)
        self.iterations = iterations
        self.rule_list = rule_list

    def replace(self):
        """ Swap operators according to the grammar rules """
        old_stack = self.stack.copy()
        self.stack.clear()
        parser = Parser()

        while self.iterations > 0:
            for operator in old_stack:
                if operator.symbol in self.rule_list.keys():
                    applicable_rules = self.rule_list[operator.symbol]
                    rule = random.choice(applicable_rules)
                    self.stack.extend(parser.map_list_of_symbols(rule))
                else:
                    self.stack.append(operator)
            self.iterations -= 1

    def apply_operator(self):
        """ Apply operator """
        operator = self.stack.popleft()
        operator.execute()
