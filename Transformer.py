from MeshOutputBuilder import MeshOutputBuilder
from Rule import Rule
from collections import deque

class Transformer:
    def __init__(self, rule_list: [Rule]):
        self.stack = deque()
        self.stack.extend(rule_list)

    def transform(self, meshBuilder: MeshOutputBuilder):

        rule = self.stack.popleft()
        rule.execute()

