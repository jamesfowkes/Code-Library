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

startFound = False

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
    
def main(argv = None):
    if argv is None:
        argv = sys.argv
    
    if len(argv) < 4:
        sys.exit("Not enough arguments supplied")
        
    mmcu = argv[1]
    avr_dir = argv[2]
    harness_dir = argv[3]
    
    path = "%s/%s.h" % (avr_dir, mmcu)

    outputPath = "%s/io.h" % harness_dir
    
    infile = open(path, 'r')
    outFile = open(outputPath, 'w')
    
    printPreamble(outFile)
    
    for l in infile:
        outFile.write( processLine(l, mmcu) )

if __name__ == "__main__":
    main()