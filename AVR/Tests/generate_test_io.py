""" Usage: generate_test_io.py <mmcu> <avr_dir> <harness_dir>

"""

###
# This file takes an AVR IO defintions file for
# a particular MCU and generates a header file
# suitable for the test harness.
#
# It basically turns any _SFR_ definitions into 'extern' declarations
# These externs reference variables that should be defined in an harness
# module for the peripheral in question
###


import sys
import docopt
import logging

startFound = False

def get_logger():
    return logging.getLogger(__name__)

def processSFR(l):
    
    identifier = l.split()
    identifier = identifier[1]
    
    if "_SFR_MEM8" in l or "_SFR_IO8" in l:
        return "extern uint8_t %s;\n#define TEST_HARNESS_%s\n" % (identifier, identifier)
    elif "_SFR_MEM16" in l or "_SFR_IO16" in l:
        return "extern uint16_t %s;\n#define TEST_HARNESS_%s\n" % (identifier, identifier)
    else:
        return ""
        
def processLine(l, mmcu):
    
    global startFound
    
    output = ""
    
    if not startFound:
        first_define = "#define _AVR_%s_H_" % mmcu.upper()
        startFound = l.startswith(first_define)
        
        if startFound:
            output = "#ifndef _AVR_%s_H_\n%s" % (mmcu.upper(), first_define)
    else:
        if "_SFR" in l:
            output = processSFR(l)
        elif "#endif" not in l or "#ifndef" not in l:
            output = l
        
    return output
    
def printPreamble(file):
    file.write('// Auto-generated by generate_test_io.py\n')
    file.write('#include "io_harness_macros.h"\n')
    
def get_args(argv):
    
    if argv is None:
        get_logger().info("Parsing with docopt")
        args = docopt.docopt(__doc__)
    else:
        try:
            args = {
                "<mmcu>": argv[0],
                "<avr_dir>": argv[1],
                "<harness_dir>": argv[2]
            }
        except:
            args = None

    return args

def generate_test_io(argv = None):

    args = get_args(argv)
    
    path = "%s/%s.h" % (args["<avr_dir>"], args["<mmcu>"])

    outputPath = "%s/io.h" % args["<harness_dir>"]
    
    infile = open(path, 'r')
    outFile = open(outputPath, 'w')
    
    printPreamble(outFile)
    
    for l in infile:
        outFile.write( processLine(l, args["<mmcu>"]) )

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    get_logger().info("Called as __main__. Args: {}".format(sys.argv))
    generate_test_io()
