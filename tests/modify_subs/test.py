import unittest
from c_delocalizer.modify_subs import find_key_by_string_wrapper

class ModifySubsTestCase(unittest.TestCase):

    def setUp(self):
        self.honorifics = {
            "honorifics": {
                "san": {
                    "kanjis": ["さん"],
                    "alternatives": ["Mr.", "Ms.", "Miss", "Mister"]
                },
                "sama": {
                    "kanjis": ["さま", "様"],
                    "alternatives": ["Lady", "Lord", "Sir", "Ma'am", "Prince", "Princess", "Great"]
                }
            }
        }
    
    def test_find_key(self):
        # Test cases to verify the behavior of the function
        k = find_key_by_string_wrapper(self.honorifics, "さん", "kanjis")
        self.assertEqual(k, "san")