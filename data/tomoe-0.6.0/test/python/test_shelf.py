# encoding: utf-8
import os
import sys
import glob
import unittest
import test_common
import tomoe

class TomoeShelfTest(unittest.TestCase):

    def testRegister(self):
        dict = tomoe.Dict("XML")
        dict_name = "test-dict"

        shelf = tomoe.Shelf()
        self.assert_(not shelf.has_dict(dict_name))

        shelf.register_dict(dict_name, dict)
        self.assertEqual(dict, shelf.get_dict(dict_name))

    def testUnregister(self):
        dict = tomoe.Dict("XML")
        dict_name = "test-dict"

        shelf = tomoe.Shelf()
        self.assert_(not shelf.has_dict(dict_name))

        shelf.register_dict(dict_name, dict)
        self.assertEqual(dict, shelf.get_dict(dict_name))

        shelf.unregister_dict(dict_name)
        self.assert_(not shelf.has_dict(dict_name))

        shelf.register_dict(dict_name, dict)
        self.assertEqual(dict, shelf.get_dict(dict_name))

        dict_unihan = tomoe.Dict("Unihan")
        shelf.register_dict(dict_name, dict_unihan)
        self.assertEqual(dict_unihan, shelf.get_dict(dict_name))

        shelf.unregister_dict(dict_name)
        self.assert_(not shelf.has_dict(dict_name))

    def testGetNames(self):
        dict1 = tomoe.Dict("XML")
        dict_name1 = "test-dict1"

        dict2 = tomoe.Dict("XML")
        dict_name2 = "test-dict2"

        shelf = tomoe.Shelf()
        self.assertEqual(0, len(shelf.get_dict_names()))

        shelf.register_dict(dict_name1, dict1)
        shelf.register_dict(dict_name2, dict2)
        self.assertEqual([dict_name1, dict_name2].sort(), shelf.get_dict_names().sort())

# vi:ts=4:nowrap:ai:expandtab
