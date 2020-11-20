# encoding: utf-8
import glob
import os
import sys
import unittest
top_dir = os.path.join ('..', '..')
sys.path.insert(0, os.path.join(top_dir, 'bindings', 'python', '.libs'))
import tomoe
tomoe.tomoe_dict_set_default_module_dir(os.path.join(top_dir, 'module', 'dict', '.libs'))
tomoe.tomoe_recognizer_set_default_module_dir(os.path.join(top_dir,  'module', 'recognizer', '.libs'))
data_dir = os.path.join(top_dir, 'data')
test_data_dir = os.path.join(top_dir, 'test', 'data')

def parseStrokeData(file):
    writing = tomoe.Writing()
    lines = open(file, 'r').readlines()
    results = lines.pop(0)
    for line in lines:
        points = line.split(',')
        first_point = points.pop(0)
        x, y = first_point.split()
        writing.move_to(int(x), int(y))
        for point in points:
            x, y = point.split()
            writing.line_to(int(x), int(y))

    return results, writing

# vi:ts=4:nowrap:ai:expandtab
