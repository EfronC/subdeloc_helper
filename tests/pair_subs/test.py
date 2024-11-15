import unittest
import json
import os
from c_delocalizer.pair_subs import find_intersections

class PairSubsTestCase(unittest.TestCase):

    def setUp(self):
        dirname = os.path.dirname(__file__)
        self.set_a = [
            {"start": 0, "end": 5000, "text":"Lorem Ipsum", "original":"Lorem Ipsum", "nl": 0},
            {"start": 5000, "end": 10000, "text":"Foo Bar", "original":"Foo Bar", "nl": 1},
        ]
        self.set_b = [
            {"start": 0, "end": 5000, "text":"Lorem Ipsum", "original":"Lorem Ipsum", "nl": 0},
            {"start": 5000, "end": 10000, "text":"Foo Bar", "original":"Foo Bar", "nl": 1},
        ]
        self.result = [{'start': 0, 'end': 5000, 'original': [{'start': 0, 'end': 5000, 'text': 'Lorem Ipsum', 'original': 'Lorem Ipsum', 'nl': 0}], 'reference': [{'start': 0, 'end': 5000, 'text': 'Lorem Ipsum', 'original': 'Lorem Ipsum', 'nl': 0}]}, {'start': 5000, 'end': 10000, 'original': [{'start': 5000, 'end': 10000, 'text': 'Foo Bar', 'original': 'Foo Bar', 'nl': 1}], 'reference': [{'start': 5000, 'end': 10000, 'text': 'Foo Bar', 'original': 'Foo Bar', 'nl': 1}]}]

        with open(os.path.join(dirname, "files/single.json"), encoding='utf-8') as f:
            test_data_single = json.load(f)

        self.set_a_single = test_data_single["set_a"]
        self.set_b_single = test_data_single["set_b"]
        self.result_single = [{'start': 0, 'end': 11000, 'reference': [{'start': 0, 'end': 11000, 'text': 'こんにちは', 'original': 'こんにちは', 'nl': 0}], 'original': [{'start': 0, 'end': 10000, 'text': 'Hello', 'original': 'Hello', 'nl': 0}]}, {'start': 14000, 'end': 20000, 'reference': [{'start': 14000, 'end': 20000, 'text': '世界殿', 'original': '世界殿', 'nl': 1}], 'original': [{'start': 15000, 'end': 20000, 'text': 'Sir World', 'original': 'Sir World', 'nl': 1}]}]

        with open(os.path.join(dirname, "files/multi.json"), encoding='utf-8') as f:
            test_data_multi = json.load(f)

        self.set_a_multi = test_data_multi["set_a"]
        self.set_b_multi = test_data_multi["set_b"]

    def test_find_intersections(self):
        # Test cases to verify the behavior of the function
        intersections = find_intersections(self.set_a, self.set_b)
        self.assertEqual(intersections, self.result)

    def test_find_intersections_single(self):
        # Test cases to verify the behavior of the function
        intersections = find_intersections(self.set_a_single, self.set_b_single)
        self.assertEqual(intersections, self.result_single)

    def test_find_intersections_multi(self):
        # Test cases to verify the behavior of the function
        intersections = find_intersections(self.set_a_multi, self.set_b_multi)
        self.assertEqual(len(intersections[0]["original"]), 2)
        self.assertEqual(len(intersections[0]["reference"]), 2)
        self.assertEqual(len(intersections[1]["original"]), 4)
        self.assertEqual(len(intersections[1]["reference"]), 4)