""" This module applies rules and executes them
after all the replacements acceptable within bounding volume
"""

import random


class LSystemSimulator:
    """ Apply grammar rules and apply operators """

    def __init__(self, parser, initial_word: str, rule_list: dict, iterations: int):
        self.parser = parser
        self.produced_word = initial_word
        self.iterations = iterations
        self.rule_list = rule_list

    def produce(self):
        """ Swap operators according to the grammar rules """
        if self.iterations > 0:
            while self.iterations > 0:
                new_word = ""
                for idx, operator in enumerate(self.produced_word):
                    if operator in self.rule_list.keys():
                        applicable_rules = self.rule_list[operator]
                        rule = random.choice(applicable_rules)
                        new_word += self.apply_rule(rule, idx)
                    else:
                        new_word += operator
                self.iterations -= 1
                self.produced_word = new_word
        return self.produced_word

    def apply_operators(self):
        """ Apply operator """
        parser = self.parser
        for symbol in self.produced_word:
            operator = parser.map_symbol(symbol)
            operator()

    def apply_rule(self, rule, split_idx):
        """ Check conditions based on context and applies rule accordingly
        Computes predecessor context and successor context to evaluate
        context rules
        """

        match_context = True  # if no context or context is right - apply rule

        if "<" in rule:
            predecessor_context = self.get_predecessor(split_idx)
            left_cond = rule.split("<")[0]  # take left side of the rule
            if left_cond not in predecessor_context:  # I assumed continuous match for now
                match_context = False
        if ">" in rule:
            successor_context = self.get_successor(split_idx)
            right_cond = rule.split("<")[1]
            if right_cond not in successor_context:
                match_context = False

        if match_context:
            return rule.split("=")[-1]  # take the rule production outcome
        return ""

    def get_predecessor(self, split_idx):
        """ Get all characters that are the predeceasing context of a given char """
        stack = []
        idx = 0
        stack[idx] = ""

        predecessor = ""
        for i in range(0, split_idx):
            char = self.produced_word[i]
            if char == "[":
                idx += 1
                stack[idx] = ""
            elif char == "]":
                stack[idx] = ""
                idx -= 1
            else:
                stack[idx] += char
        for item in stack:
            predecessor += item
        return predecessor

    def get_successor(self, split_idx):
        """ Get all successor context starting from split_idx """
        brackets = 0
        successor = ""
        for i in range(0, split_idx):
            char = self.produced_word[i]
            if char == "[":
                brackets += 1
            elif char == "]":
                brackets -= 1

        idx = split_idx
        length = len(self.produced_word)
        while brackets > 0 and idx < length:
            char = self.produced_word[idx]
            idx += 1
            if char == "[":
                brackets += 1
            elif char == "]":
                brackets -= 1
            else:
                successor += char
        return successor
