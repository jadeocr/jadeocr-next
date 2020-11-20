# encoding: utf-8
import os
import sys
import glob
import unittest
import test_common
import tomoe

class TomoeCandidateTest(unittest.TestCase):

    def testScore(self):
        char_code = '池'
        tomoe_char = tomoe.Char(utf8 = char_code)

        score = 1
        cand = tomoe.Candidate(tomoe_char)
        self.assertEqual(0, cand.get_score())
        cand.set_score(score)
        self.assertEqual(score, cand.get_score())

    def testGetChar(self):
        char_code = '池'
        tomoe_char = tomoe.Char(utf8 = char_code)
        cand = tomoe.Candidate(tomoe_char)
        self.assertEqual(char_code, cand.get_char().get_utf8())

    def testCompare(self):
        char_code = '池'
        tomoe_char1 = tomoe.Char(utf8 = char_code)
        cand1 = tomoe.Candidate(tomoe_char1)
        cand1.set_score(1)
        
        tomoe_char2 = tomoe.Char(utf8 = char_code)
        cand2 = tomoe.Candidate(tomoe_char2)
        cand2.set_score(2)
        self.assert_(cand1.compare(cand2) < 0)


# vi:ts=4:nowrap:ai:expandtab
