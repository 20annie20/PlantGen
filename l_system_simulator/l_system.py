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

        left_context = None

        for condition in rule:
            # if no context or context is right - apply rule
            if "<" in condition:
                left_cond = condition.split("<")[0]  # take left side of the rule
                if left_context is None:
                    left_context = self.get_left_context(split_idx, ignore, ignore_chars)
                if len(left_cond) > len(left_context):
                    continue
                if left_cond != left_context[-len(left_cond):]:
                    continue  # context not matched
            if ">" in condition:
                right_cond = condition.split(">")[1].split("=")[0]
                right_contexts = list(self.get_right_context(split_idx + 1, ignore, ignore_chars, len(right_cond)))
                if right_cond not in right_contexts:
                    continue
            return condition.split("=")[-1]  # take the rule production outcome

        return self.produced_word[split_idx]

    def get_left_context(self, split_idx, ignore, ignore_chars):
        """ Get strict left context of a given char """
        stack = [""]
        stack_idx = 0

        for i in range(0, split_idx):
            char = self.produced_word[i]
            if ignore:
                if char in ignore_chars:
                    continue
            if char == "[":
                stack_idx += 1
                if stack_idx == len(stack):
                    stack.append("")
            elif char == "]":
                stack[stack_idx] = ""
                stack_idx -= 1
            else:
                stack[stack_idx] += char

        last = "".join(stack)
        return last

    def get_right_context(self, split_idx, ignore, ignore_chars, size):
        """ Get strict right contexts starting from split_idx """
        """ Each branch included adds one new context """
        depth = 0
        branch_ctxs = [""]

        valid_idx = split_idx
        length = len(self.produced_word)

        if ignore:
            while valid_idx < length:
                if self.produced_word[valid_idx] not in ignore_chars:
                    break
                valid_idx += 1


        idx = valid_idx
        if size > 0:
            while idx < length:
                char = self.produced_word[idx]
                if ignore:
                    if char in ignore_chars:
                        idx += 1
                        continue

                if char == "[":
                    if depth < size:
                        depth += 1
                        branch_ctxs.append("")
                    else:
                        while depth >= size:
                            idx += 1
                            char = self.produced_word[idx]
                            if char == "]":
                                depth -= 1

                elif char == "]":
                    depth -= 1
                    if depth < 0:
                        break
                else:
                    if len(branch_ctxs[depth]) < size:
                        branch_ctxs[depth] += char
                idx += 1

        # scalanie kontekstów w razie dłuższych kontekstow wymagane
        return branch_ctxs
