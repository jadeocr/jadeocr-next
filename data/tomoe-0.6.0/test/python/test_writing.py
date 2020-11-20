# encoding: utf-8
import os
import sys
import glob
import unittest
import test_common
import tomoe

class TomoeWritingTest(unittest.TestCase):

    def testDuplicate(self):
        writing1 = tomoe.Writing()
        n = 10
        for i in range(n):
            writing1.move_to(i, i)
            writing1.line_to(i * 2, i * 2)
        
        writing2 = writing1.dup()
        self.assertEqual(writing1.to_xml(), writing2.to_xml())

    def testClear(self):
        writing = tomoe.Writing()

        self.assertEqual(0, len(writing.get_strokes()))
        n = 10
        for i in range(n):
            writing.move_to(i, i)
            writing.line_to(i * 2, i * 2)

        writing.clear()
        self.assertEqual(0, len(writing.get_strokes()))

    def testGetNStrokes(self):
        writing = tomoe.Writing()
        self.assertEqual(0, len(writing.get_strokes()))

        n = 10
        for i in range(n):
            writing.move_to(i, i)
            writing.line_to(i * 2, i * 2)
        self.assertEqual(n, writing.get_n_strokes())

    def testGetStrokes(self):
        writing = tomoe.Writing()
        self.assertEqual(0, len(writing.get_strokes()))

        n = 10
        for i in range(n):
            writing.move_to(int(i), int(i))
            writing.line_to(int(i * 2), int(i * 2))
        self.assertEqual(n, writing.get_n_strokes())
       
        for i, stroke in enumerate(writing.get_strokes()):
            for j, point in enumerate(stroke):
                self.assertEqual(point, (i * (j+1), i * (j+1)))

    def testRemoveLastStroke(self):
        writing = tomoe.Writing()
        self.assertEqual(0, len(writing.get_strokes()))

        n = 10
        for i in range(n):
            writing.move_to(i, i)
            writing.line_to(i * 2, i * 2)
        self.assertEqual(n, writing.get_n_strokes())
        writing.remove_last_stroke()
        
        self.assertEqual(n - 1, writing.get_n_strokes())
        for i, stroke in enumerate(writing.get_strokes()):
            for j, point in enumerate(stroke):
                self.assertEqual((i * (j+1), i * (j+1)), point)

    def testToXML(self):
        writing = tomoe.Writing()
        n = 10
        for i in range(n):
            writing.move_to(i, i)
            writing.line_to(i * 2, i * 2)

        xml = ""
        xml += "    <strokes>\n"
        for i, stroke in enumerate(writing.get_strokes()):
            xml += "      <stroke>\n"
            for j, point in enumerate(stroke):
                xml += "        <point x=\"%d\" y=\"%d\"/>\n" % (i * (j+1), i * (j+1))
            xml += "      </stroke>\n"
        xml += "    </strokes>\n"
        self.assertEqual(xml, writing.to_xml())

# vi:ts=4:nowrap:ai:expandtab
