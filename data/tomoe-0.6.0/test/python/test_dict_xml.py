# encoding: utf-8
import os
import sys
import shutil
import unittest
import test_common
import tomoe
import test_dict

class TomoeDictXMLTest(test_dict.TomoeDictTest):
    def setUp(self):
        test_dict_name = os.path.join(test_common.test_data_dir, 'test-dict.xml')
        self.dict_name = "tomoe-test-xmldict.xml"
        shutil.copy(test_dict_name, self.dict_name)
        self.dict = tomoe.Dict("XML", filename = self.dict_name, editable = True)

    def tearDown(self):
        self.dict.flush()
        self.dict = None
        if os.access(self.dict_name, os.F_OK):
            os.unlink(self.dict_name)

# vi:ts=4:nowrap:ai:expandtab
