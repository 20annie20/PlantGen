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

    def produce(self, ignore=False, ignore_chars=""):
        """ Swap operators according to the grammar rules """
        if self.iterations > 0:
            while self.iterations > 0:
                new_word = ""
                for idx, operator in enumerate(self.produced_word):
                    if operator in self.rule_list.keys():
                        applicable_rules = self.rule_list[operator]
                        rule = random.choice(applicable_rules)
                        new_word += self.apply_rule(rule, idx, ignore, ignore_chars)
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

    def apply_rule(self, rule, split_idx, ignore, ignore_chars):
        """ Check conditions based on context and applies rule accordingly
        Computes predecessor context and successor context to evaluate
        context rules
        """

        match_context = True  # if no context or context is right - apply rule
        for condition in rule:
            if "<" in condition:
                predecessor_context = self.get_predecessor(split_idx, ignore, ignore_chars)
                left_cond = condition.split("<")[0]  # take left side of the rule
                if left_cond != predecessor_context:  # I assumed continuous match for now
                    match_context = False
            if ">" in condition:
                successor_context = self.get_successor(split_idx, ignore, ignore_chars)
                right_cond = condition.split("<")[1]
                if right_cond != successor_context:
                    match_context = False

            if match_context:
                return rule.split("=")[-1]  # take the rule production outcome
        return ""

    def get_predecessor(self, split_idx, ignore, ignore_chars):
        """ Get strict predecessor context of a given char """
        stack = [""]
        idx = 0

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

        last = stack[-1]
        for i in reversed(last):
            if ignore:
                if i not in ignore_chars:
                    return i
            else:
                return i
        return ""

    def get_successor(self, split_idx, ignore, ignore_chars):
        """ Get strict successor context starting from split_idx """
        brackets = 0
        successor = ""

        idx = split_idx
        length = len(self.produced_word)

        while brackets > 0 and idx < length:
            char = self.produced_word[idx]
            idx += 1
            if char == "[":
                brackets += 1
            elif char == "]":
                brackets -= 1
            elif brackets <= 0:
                if ignore:
                    if successor not in ignore_chars:
                        successor = char
                        break
                else:
                    successor = char
        return successor
