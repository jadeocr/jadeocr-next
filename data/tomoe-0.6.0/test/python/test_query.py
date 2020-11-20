# encoding: utf-8
import os
import sys
import unittest
import tomoe

class TomoeQueryTest(unittest.TestCase):

    def testUTF8(self):
        utf8 = '池'
        query = tomoe.Query()
        self.assertEqual(None, query.get_utf8())
        query.set_utf8(utf8)
        self.assertEqual(utf8, query.get_utf8())

        query = tomoe.Query(utf8 = utf8)
        self.assertEqual(utf8, query.get_utf8())

    def testReading(self):
        query = tomoe.Query()
        self.assertEqual(0, len(query.get_readings()))

        reading_string = "いけ"
        reading1 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        query.add_reading(reading1)
        self.assertEqual([reading1], query.get_readings())

        reading_string = "ひげ"
        reading2 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        query.add_reading(reading2)
        self.assertEqual([reading1, reading2].sort(), query.get_readings().sort())

    def testRadical(self):
        query = tomoe.Query()
        self.assertEqual(0, len(query.get_radicals()))

        radical1 = "いけ"
        query.add_radical(radical1)
        self.assertEqual([radical1], query.get_radicals())

        radical2 = "ひげ"
        query.add_radical(radical2)
        self.assertEqual([radical1, radical2].sort(), query.get_radicals().sort())

    def testRadical(self):
        query = tomoe.Query()
        self.assertEqual(0, len(query.get_radicals()))

        radical1 = "いけ"
        query.add_radical(radical1)
        self.assertEqual([radical1], query.get_radicals())

        radical2 = "ひげ"
        query.add_radical(radical2)
        self.assertEqual([radical1, radical2].sort(), query.get_radicals().sort())

    def testVariant(self):
        query = tomoe.Query()
        self.assertEqual(None, query.get_variant())

        variant = "池"
        query.set_variant(variant)
        self.assertEqual(variant, query.get_variant())

        variant = "地"
        query.set_variant(variant)
        self.assertEqual(variant, query.get_variant())

    def testMinStroke(self):
        query = tomoe.Query()
        self.assertEqual(-1, query.get_min_n_strokes())

        n_strokes = 2
        query.set_min_n_strokes(n_strokes)
        self.assertEqual(n_strokes, query.get_min_n_strokes())

        n_strokes = 3
        query.set_min_n_strokes(n_strokes)
        self.assertEqual(n_strokes, query.get_min_n_strokes())

    def testMaxStroke(self):
        query = tomoe.Query()
        self.assertEqual(-1, query.get_max_n_strokes())

        n_strokes = 2
        query.set_max_n_strokes(n_strokes)
        self.assertEqual(n_strokes, query.get_max_n_strokes())

        n_strokes = 3
        query.set_max_n_strokes(n_strokes)
        self.assertEqual(n_strokes, query.get_max_n_strokes())

    def testEmpty(self):
        query = tomoe.Query()
        self.assert_(query.is_empty())

        query = tomoe.Query(utf8 = '池')
        self.assert_(not query.is_empty())

# vi:ts=4:nowrap:ai:expandtab
