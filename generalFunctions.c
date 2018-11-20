#include "ConsoleUI.h"

void freeOldFile(grayImage* img, grayImage *colorSegmentsImg, imgPosList *segments, int sizeimgPosList, bool loadFlag, bool segmentsFlag, bool coloredFlag)
{
	if (loadFlag)
		freeGrayImage(img);
	if (segmentsFlag)
		freeImgPosArray(segments, sizeimgPosList);
	if (coloredFlag)
		freeGrayImage(colorSegmentsImg);
}
void* checkMemoryAllocations(void* ptr)
{ // the function check memory allocations
	if (ptr == NULL)
	{
		printf("%s", "Allocation failed\n");
		exit(1);
	}
	return ptr;
}
FILE* checkFileOpen(FILE *fp, char * fileName)
{ //the function checks if file open successed
	if (!fp)
	{
		printf("The file '%s' can not been open\n", fileName);
		exit(1);
	}
	return fp;
}