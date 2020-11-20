# encoding: utf-8
import os
import sys
import unittest
import tomoe
import test_dict

class TomoeDictUnihanTest(test_dict.TomoeDictTest):
    def setUp(self):
        self.dict = tomoe.Dict("Unihan")

    def testRegisterChar(self):
        return

    def testUnregisterChar(self):
        return

    def testUnknownReadingSearch(self):
        """
        Unihan dicitionary has no unknown-reading character
        """
        return

    def testOnUnknownReadingSearch(self):
        """
        Unihan dicitionary has no unknown-reading character
        """
        return

    def testKunUnknownReadingSearch(self):
        """
        Unihan dicitionary has no unknown-reading character
        """
        return

    def testUnknownKunReadingSearch(self):
        char_codes = [ '池', '垳', '汪', '溏', '潢' ]
        reading = 'いけ'
        tomoe_reading = tomoe.Reading(tomoe.READING_UNKNOWN, reading)
        tomoe_query = tomoe.Query()
        tomoe_query.add_reading(tomoe_reading)

        candidates = self.dict.search(tomoe_query)
        candidate_chars = map(lambda x: x.get_char().get_utf8() , candidates)
        for char, candidate_char in zip(sorted(char_codes), sorted(candidate_chars)):
            self.assertEqual(char, candidate_char)

    def testKunReadingSearch(self):
        char_codes = [ '池', '垳', '汪', '溏', '潢' ]
        reading = 'いけ'
        tomoe_reading = tomoe.Reading(tomoe.READING_JA_KUN, reading)
        tomoe_query = tomoe.Query()
        tomoe_query.add_reading(tomoe_reading)

        candidates = self.dict.search(tomoe_query)
        candidate_chars = map(lambda x: x.get_char().get_utf8() , candidates)
        for char, candidate_char in zip(sorted(char_codes), sorted(candidate_chars)):
            self.assertEqual(char, candidate_char)

    def testOnReadingSearch(self):
        char_codes = [ '垳' ]
        reading = 'ガケ'
        tomoe_reading = tomoe.Reading(tomoe.READING_JA_ON, reading)
        tomoe_query = tomoe.Query()
        tomoe_query.add_reading(tomoe_reading)

        candidates = self.dict.search(tomoe_query)
        candidate_chars = map(lambda x: x.get_char().get_utf8() , candidates)
        for char, candidate_char in zip(sorted(char_codes), sorted(candidate_chars)):
            self.assertEqual(char, candidate_char)

    def testUnknownOnReadingSearch(self):
        char_codes = [ '垳' ]
        reading = 'ガケ'
        tomoe_reading = tomoe.Reading(tomoe.READING_JA_ON, reading)
        tomoe_query = tomoe.Query()
        tomoe_query.add_reading(tomoe_reading)

        candidates = self.dict.search(tomoe_query)
        candidate_chars = map(lambda x: x.get_char().get_utf8() , candidates)
        for char, candidate_char in zip(sorted(char_codes), sorted(candidate_chars)):
            self.assertEqual(char, candidate_char)

# vi:ts=4:nowrap:ai:expandtab
