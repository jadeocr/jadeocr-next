# encoding: utf-8
import os
import sys
import unittest
import tomoe

class TomoeCharTest(unittest.TestCase):

    def testDuplicate(self):
        data = """
<character>
  <utf8>池</utf8>
  <number-of-strokes>6</number-of-strokes>
  <readings>
    <reading type="ja_on">タ</reading>
    <reading type="ja_on">チ</reading>
    <reading type="ja_kun">いけ</reading>
    <reading type="ja_unknown">あんのうん</reading>
  </readings>
  <meta>
    <jis208>35-51</jis208>
    <ucs>6c60</ucs>
    <jouyou>2</jouyou>
    <meaning>pond, cistern, pool, reservoir</meaning>
  </meta>
</character>
        """
        char1 = tomoe.tomoe_char_new_from_xml_data(data, int(-1))
        self.assertNotEqual(None, char1)
        char2 = char1.dup()
        self.assertEqual(char1.to_xml(), char2.to_xml())

    def testUTF8(self):
        utf8 = '池'

        tomoe_char = tomoe.Char()
        self.assertEqual(None, tomoe_char.get_utf8())
        tomoe_char.set_utf8(utf8)
        self.assertEqual(utf8, tomoe_char.get_utf8())

        tomoe_char = tomoe.Char(utf8 = utf8)
        self.assertEqual(utf8, tomoe_char.get_utf8())

        new_utf8 = '地'
        tomoe_char.set_utf8(new_utf8)
        self.assertEqual(new_utf8, tomoe_char.get_utf8())

    def testStrokes(self):
        n_strokes = 2
        tomoe_char = tomoe.Char()
        self.assertEqual(-1, tomoe_char.get_n_strokes())
        tomoe_char.set_n_strokes(n_strokes)
        self.assertEqual(n_strokes, tomoe_char.get_n_strokes())

        tomoe_char = tomoe.Char(n_strokes = n_strokes)
        self.assertEqual(n_strokes, tomoe_char.get_n_strokes())

        n_strokes = 3
        tomoe_char.set_n_strokes(n_strokes)
        self.assertEqual(n_strokes, tomoe_char.get_n_strokes())

    def testReading(self):
        reading_string = 'いけ'
        reading1 = tomoe.Reading(tomoe.READING_UNKNOWN, reading_string)

        tomoe_char = tomoe.Char()
        self.assertEqual(0, len(tomoe_char.get_readings()))

        tomoe_char.add_reading(reading1)
        self.assertEqual([reading1], tomoe_char.get_readings())

        reading2 = tomoe.Reading(tomoe.READING_JA_KUN, reading_string)
        tomoe_char.add_reading(reading2)
        self.assertEqual([reading1, reading2].sort(), tomoe_char.get_readings().sort())

    def testRadical(self):
        radical_string1 = 'いけ'

        tomoe_char = tomoe.Char()
        self.assertEqual(0, len(tomoe_char.get_radicals()))

        tomoe_char.add_radical(radical_string1)
        self.assertEqual([radical_string1], tomoe_char.get_radicals())

        radical_string2 = 'ほげ'
        tomoe_char.add_radical(radical_string2)
        self.assertEqual([radical_string1, radical_string2].sort(), tomoe_char.get_radicals().sort())

    def testWriting(self):
        writing = tomoe.Writing()

        tomoe_char = tomoe.Char()
        self.assertEqual(None, tomoe_char.get_writing())
        tomoe_char.set_writing(writing)
        self.assertEqual(writing, tomoe_char.get_writing())

        tomoe_char = tomoe.Char(writing = writing)
        self.assertEqual(writing, tomoe_char.get_writing())

        writing.move_to(1, 1)
        writing.line_to(2, 2)
        tomoe_char.set_writing(writing)
        self.assertEqual(writing.to_xml(), tomoe_char.get_writing().to_xml())

    def testVariant(self):
        variant = '池'
        tomoe_char = tomoe.Char()
        self.assertEqual(None, tomoe_char.get_variant())
        tomoe_char.set_variant(variant)
        self.assertEqual(variant, tomoe_char.get_variant())

        tomoe_char = tomoe.Char(variant = variant)
        self.assertEqual(variant, tomoe_char.get_variant())

        variant = '地'
        tomoe_char.set_variant(variant)
        self.assertEqual(variant, tomoe_char.get_variant())

    def testCompare(self):
        char1 = '池'
        char2 = '地'
        
        tomoe_char1 = tomoe.Char(utf8 = char1)
        tomoe_char2 = tomoe.Char(utf8 = char1)
        self.assertEqual(tomoe_char1, tomoe_char2)
        
        tomoe_char2 = tomoe.Char(utf8 = char2)
        self.assertNotEqual(0, tomoe_char1, tomoe_char2)

# vi:ts=4:nowrap:ai:expandtab
