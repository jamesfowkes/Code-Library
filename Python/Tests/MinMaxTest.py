import unittest

import MinMax

class MinMaxTest(unittest.TestCase):

    def setUp(self):
        self.minmax = MinMax.MinMax()
        
    def testNone(self):
        self.assertIsNone(self.minmax.min)
        self.assertIsNone(self.minmax.max)
        
    def testAddSingleValueMakesMinMaxEqual(self):
        self.minmax.newValue(0)
        self.assertEqual(self.minmax.min, 0)
        self.assertEqual(self.minmax.max, 0)

    def testAddSeveralValuesAndTracksMinMax(self):
    
        self.minmax.newValue(0)
        
        self.minmax.newValue(-1)
        self.assertEqual(self.minmax.min, -1)
        self.assertEqual(self.minmax.max, 0)
        
        self.minmax.newValue(1)
        self.assertEqual(self.minmax.min, -1)
        self.assertEqual(self.minmax.max, 1)
        
        self.minmax.newValue(-100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 1)
        
        self.minmax.newValue(100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 100)

    def testResetSetsBothValuesCorrectly(self):
        self.minmax.reset()
        self.assertIsNone(self.minmax.min)
        self.assertIsNone(self.minmax.max)
        
        self.minmax.reset(-100, 100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 100)
        
    def testInitWithPredefinedValuesSetsMinMax(self):
    
        self.minmax = MinMax.MinMax(-100, 100)
        self.assertEqual(self.minmax.min, -100)
        self.assertEqual(self.minmax.max, 100)
       
    def testInitWithBadValuesRaisesException(self):
        self.assertRaises(MinMax.MinMaxError, self.badInit)
        
    def testResetWithBadValuesRaisesException(self):
        self.assertRaises(MinMax.MinMaxError, self.badReset)    
        
    def badInit(self):
        self.minmax = MinMax.MinMax(1, -1)
        
    def badReset(self):
        self.minmax.reset(1, -1)