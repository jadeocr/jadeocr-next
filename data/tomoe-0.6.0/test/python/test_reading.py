# encoding: utf-8
import os
import sys
import unittest
import tomoe

class TomoeReadingTest(unittest.TestCase):

    def testDuplicate(self):
        for type in (tomoe.READING_INVALID, tomoe.READING_UNKNOWN, tomoe.READING_JA_KUN, tomoe.READING_JA_ON):
            reading_string = 'いけ'
            reading1 = tomoe.Reading(type, reading_string)
            reading2 = reading1.dup()
            self.assertEqual(type, reading2.get_reading_type())
            self.assertEqual(reading_string, reading2.get_reading())

    def testGetReadingType(self):
        reading_string = "いけ"

        reading = tomoe.Reading(tomoe.READING_INVALID, reading_string)
        self.assertEqual(tomoe.READING_INVALID, reading.get_reading_type())

        reading = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        self.assertEqual(tomoe.READING_UNKNOWN, reading.get_reading_type())

        reading = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        self.assertEqual(tomoe.READING_JA_KUN, reading.get_reading_type())

        reading = tomoe.Reading(tomoe.READING_JA_ON, reading_string)
        self.assertEqual(tomoe.READING_JA_ON, reading.get_reading_type())

    def testGetReading(self):
        reading_string = "いけ"
        reading = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        self.assertEqual(reading_string, reading.get_reading())

    def testToXML(self):
        reading_string = "いけ"
        reading = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        reading_data = """\
      <reading type="unknown">%s</reading>
""" % (reading_string)
        self.assertEqual(reading_data, reading.to_xml())

    def testCompare(self):
        reading_string = "いけ"
        reading_string2 = "ほげ"
        reading1 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        reading2 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        reading3 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string2)
        self.assertEqual(reading1, reading2)
        self.assertNotEqual(reading1, reading3)

        reading1 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        reading2 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        reading3 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string2)
        reading4 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string2)
        self.assertEqual(reading1, reading2)
        self.assertNotEqual(reading2, reading3)
        self.assertNotEqual(reading1, reading4)

        reading1 = tomoe.Reading(tomoe.READING_JA_ON, reading_string)
        reading2 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)
        reading3 = tomoe.Reading(tomoe.READING_JA_ON, reading_string2)
        reading4 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string2)
        self.assertEqual(reading1, reading2)
        self.assertNotEqual(reading2, reading3)
        self.assertNotEqual(reading1, reading4)

        reading1 = tomoe.Reading(tomoe.READING_JA_ON, reading_string)
        reading2 = tomoe.Reading(tomoe.READING_JA_ON, reading_string)
        reading3 = tomoe.Reading(tomoe.READING_JA_ON, reading_string2)
        self.assertEqual(reading1, reading2)
        self.assertNotEqual(reading1, reading3)

        reading1 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        reading2 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        reading3 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string2)
        self.assertEqual(reading1, reading2)
        self.assertNotEqual(reading1, reading3)

        reading1 = tomoe.Reading(tomoe.READING_JA_ON, reading_string)
        reading2 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        reading3 = tomoe.Reading(tomoe.READING_JA_ON, reading_string2)
        reading4 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string2)
        self.assertNotEqual(reading1, reading2)
        self.assertNotEqual(reading2, reading3)
        self.assertNotEqual(reading1, reading4)

# vi:ts=4:nowrap:ai:expandtab
