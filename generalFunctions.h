#ifndef generalFunctions
#define generalFunctions
#include "header.h"

void * checkMemoryAllocations(void* ptr);
FILE * checkFileOpen(FILE *fp, char * fileName);
void freeOldFile(grayImage* img, grayImage *colorSegmentsImg, imgPosList *segments, int sizeimgPosList, bool loadFlag, bool segmentsFlag, bool coloredFlag);

#endif