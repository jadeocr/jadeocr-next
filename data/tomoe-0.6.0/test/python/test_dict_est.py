# encoding: utf-8
import os
import sys
import shutil
import unittest
import tomoe
import test_dict

class TomoeDictEstTest(test_dict.TomoeDictTest):

    def setUp(self):
        self.dict_name = "est-dict"
        self.dict = tomoe.Dict('Est', name = self.dict_name, database = self.dict_name, editable = True)

        tomoe_char = tomoe.Char(utf8 = '池', n_strokes = 6)
        reading = tomoe.Reading(tomoe.READING_JA_KUN, 'いけ')
        tomoe_char.add_reading(reading)
        reading = tomoe.Reading(tomoe.READING_JA_ON, 'チ')
        tomoe_char.add_reading(reading)
        reading = tomoe.Reading(tomoe.READING_JA_ON, 'タ')
        tomoe_char.add_reading(reading)
        reading = tomoe.Reading(tomoe.READING_UNKNOWN, 'あんのうん')
        tomoe_char.add_reading(reading)

        self.dict.register_char(tomoe_char)
        self.dict.flush()

    def tearDown(self):
        self.dict.flush()
        self.dict = None
        if os.access(self.dict_name, os.F_OK):
            shutil.rmtree(self.dict_name)

# vi:ts=4:nowrap:ai:expandtab
