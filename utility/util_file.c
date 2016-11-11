/*
 * Standard library includes
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
 * Utility Library Includes
 */

#include "util_file.h"

/*
 * Public Functions
 */

size_t getFileSize(FILE * pFile)
{
	long lSize = -1;

	if (pFile)
	{
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);	
	}

	return (lSize > -1) ? (size_t)lSize : (size_t)0;
}