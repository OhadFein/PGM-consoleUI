#ifndef header
#define header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////
/// Macros
/////////////////////////////////////////////////

#define bitsInByte (8 * sizeof(unsigned char))
#define PIXEL_FOUND_IN_BUCKET_MAP 1
#define PIXEL_NOT_FOUND_IN_BUCKET_MAP 0
#define MAX(a,b) (((a)>(b))?(a):(b))
#define true 1
#define false 0
#define OPTION_1 '1'
#define OPTION_2 '2'
#define OPTION_3 '3'
#define OPTION_4 '4'
#define OPTION_5 '5'
#define OPTION_6 '6'
#define OPTION_7 '7'
typedef unsigned char BYTE;
typedef int bool;

/////////////////////////////////////////////////
/// structs, prototypes
/////////////////////////////////////////////////

#include "structs.h"
#include "prototypes.h"

#endif