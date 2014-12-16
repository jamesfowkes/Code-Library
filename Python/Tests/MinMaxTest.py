"""
minmaxtest
Tests the functionality of the minmax module
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""


import unittest
import minmax

#pylint: disable=too-many-public-methods
class MinMaxTest(unittest.TestCase):

    """
    Tests the functionality of the minmax module
    """

    def setUp(self):
        """ Initalises minmax object before test """
        self.minmax = minmax.MinMax()

    def test_none(self):
        """ Test that with no additional arguments, both min and max are None """
        self.assertIsNone(self.minmax.min)
        self.assertIsNone(self.minmax.max)

    def test_add_single_value_makes_min_max_equal(self):
        """ Test that add a single value makes min and max equal """
        self.minmax.new_value(0)
        self.assertEqual(self.minmax.min, 0)
        self.assertEqual(self.minmax.max, 0)

    def test_add_several_values_and_tracks_min_max(self):
        """ Test that adding multiple values makes min and max track correctly """
        self.minmax.new_value(0)

        self.minmax.new_value(-1)
        self.assertEqual(self.minmax.min, -1)
        self.assertEqual(self.minmax.max, 0)

        self.minmax.new_value(1)
        self.assertEqual(self.minmax.min, -1)
        self.assertEqual(self.minmax.max, 1)

        self.minmax.new_value(-100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 1)

        self.minmax.new_value(100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 100)

    def test_reset_sets_both_values_correctly(self):
        """ Test that resetting correctly re-initialises the object """
        self.minmax.reset()
        self.assertIsNone(self.minmax.min)
        self.assertIsNone(self.minmax.max)

        self.minmax.reset(-100, 100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 100)

    def test_init_with_predefined_values_sets_min_max(self):
        """ Test that initialising with predefined values correctly initialises the object """
        self.minmax = minmax.MinMax(-100, 100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 100)

    def test_init_with_bad_values_raises_exception(self):
        """ Test that initialising with bad values raises an exception """
        with self.assertRaises(Exception):
            self.minmax = minmax.MinMax(1, -1)

    def test_reset_with_bad_values_raises_exception(self):
        """ Test that reset with bad values raises an exception """
        with self.assertRaises(Exception):
            self.minmax.reset(1, -1)
