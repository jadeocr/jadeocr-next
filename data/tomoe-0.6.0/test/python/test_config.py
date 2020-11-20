# encoding: utf-8
import os
import sys
import glob
import unittest
import test_common
import tomoe

class TomoeConfigTest(unittest.TestCase):

    def setUp(self):
        self.config_filename = "test-config"
        config_file = open(self.config_filename, "w")
        contents = """
[config]
use-system-dictionaries = false
user-dictionary = user-dict
languages = ja;zh_CN

[test1-dictionary]
type = xml
file = ../../data/kanjidic2.xml

[test2-dictionary]
type = xml
file = ../../data/kanjidic2.xml
         """
        config_file.write(contents)
        config_file.close()
        self.config = tomoe.Config(self.config_filename)

    def tearDown(self):
        if os.access(self.config_filename, os.F_OK):
            os.unlink(self.config_filename)
        
    def testGetFilename(self):
        self.assertEqual(self.config_filename, self.config.get_filename())

    def testGetUserDictName(self):
        self.assertEqual('user-dict', self.config.get_user_dict_name())

    def testGetLanguages(self):
        self.assertEqual(sorted(['ja', 'zh_CN']), sorted(self.config.get_languages()))

    def testMakeShelf(self):
        shelf = self.config.make_shelf('')
        self.assertEqual(sorted(['test1', 'test2']), sorted(shelf.get_dict_names()))

# vi:ts=4:nowrap:ai:expandtab
