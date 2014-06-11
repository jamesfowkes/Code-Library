import time

unix_time = str(int(time.time()))
unix_time_chars = ','.join(list(unix_time))

with open("compiletime.h", "w") as w:
    w.write('''
	/* This file is autogenerated by compiletime.py */
	#ifndef _COMPILE_TIME_H_
	#define _COMPILE_TIME_H_
	
	// Compile time as UNIX epoch time
	#define COMPILE_TIME_INT (UNIX_TIMESTAMP)%s
	
	//Compile time as array of digits
	#define COMPILE_TIME_DIGITS { %s }
	
	#endif
	
	''' % (unix_time, unix_time_chars)
    )