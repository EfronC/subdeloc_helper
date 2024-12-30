import unittest
import os
from c_delocalizer.pair_subs import PyIntervalSearch

class IntervalSearchTestCase(unittest.TestCase):

    def setUp(self):
        dirname = os.path.dirname(__file__)
        self.set_a = [
          {"start": 0, "end": 5000, "text": "Foo", "original": "Bar", "nl": 0},
          {"start": 6000, "end": 11000, "text": "Sample", "original": "Example", "nl": 1},
          {"start": 12000, "end": 18000, "text": "Hello", "original": "World", "nl": 2},
          {"start": 19000, "end": 24000, "text": "Test", "original": "Case", "nl": 3},
          {"start": 25000, "end": 31000, "text": "Alpha", "original": "Beta", "nl": 4},
          {"start": 32000, "end": 37000, "text": "Gamma", "original": "Delta", "nl": 5},
          {"start": 38000, "end": 43000, "text": "Quick", "original": "Brown", "nl": 6},
          {"start": 44000, "end": 49000, "text": "Fox", "original": "Jumped", "nl": 7},
          {"start": 50000, "end": 55000, "text": "Lazy", "original": "Dog", "nl": 8},
          {"start": 56000, "end": 61000, "text": "End", "original": "Start", "nl": 9}
        ]
        self.result = [
            {"start": 19000, "end": 24000, "text": "Test", "original": "Case", "nl": 3},
            {"start": 25000, "end": 31000, "text": "Alpha", "original": "Beta", "nl": 4},
            {"start": 32000, "end": 37000, "text": "Gamma", "original": "Delta", "nl": 5}
        ]
        self.result2 = [{"start": 25000, "end": 31000, "text": "Alpha", "original": "Beta", "nl": 4}]

    def test_find_intersections(self):
        # Test cases to verify the behavior of the function
        searcher = PyIntervalSearch(self.set_a)
        result = searcher.find_overlapping_intervals(17000, 39000)
        self.assertEqual(result, self.result)

    def test_find_intersections_2(self):
        # Test cases to verify the behavior of the function
        searcher = PyIntervalSearch(self.set_a)
        result = searcher.find_overlapping_intervals(24500, 29000)
        self.assertEqual(result, self.result2)