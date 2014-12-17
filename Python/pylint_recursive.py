#! /usr/bin/env python
'''
Module that runs pylint on all python scripts found in a directory tree..
'''

import os
import re
import sys

def check(module):
    '''
    apply pylint to the file specified if it is a *.py file
    '''

    module_score = 0.0

    print("CHECKING %s" % module)
    pout = os.popen('pylint %s'% module, 'r')
    for line in pout:
        if  re.match("E....:.", line):
            print(line)
        if "Your code has been rated at" in line:
            print(line)
            score = re.findall("\d{1,2}.\d\d", line)[0] #pylint: disable=anomalous-backslash-in-string
            module_score += float(score)

    return module_score

def main():

    """ Runs check function on all .py files found """
    total_score = 0.0
    module_count = 0

    try:
        print(sys.argv)
        base_directory = sys.argv[1]
    except IndexError:
        print("no directory specified, defaulting to current working directory")
        base_directory = os.getcwd()

    print("looking for *.py scripts in subdirectories of %s" % base_directory)
    for root, _, files in os.walk(base_directory):
        for name in files:
            if name[-3:] == ".py":
                filepath = os.path.join(root, name)
                total_score += check(filepath)
                module_count += 1

    print("==" * 50)
    print("%d modules found" % module_count)
    print("AVERAGE SCORE = %.02f" % (total_score / module_count))

if __name__ == "__main__":
    main()
