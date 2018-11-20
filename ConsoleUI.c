#include "ConsoleUI.h"

void run()
{
	bool loadFlag = false, segmentsFlag = false, coloredFlag = false;
	char fileName[FILENAME_MAX + 1] = { 0 };
	char compressFileName[FILENAME_MAX + 1] = { 0 };
	char fileConverted[FILENAME_MAX + 1] = { 0 };
	int sizeImgPosList, choice, threshold, max_gray_level;
	grayImage *img, *colorSegmentsImg;
	imgPosList *segments;

	printMenu();
	while ((choice = getchar()) != OPTION_7)
	{
		switch (choice)
		{
			case OPTION_1:
				if (loadFlag)
				{
					freeOldFile(img, colorSegmentsImg, segments, sizeImgPosList, loadFlag, segmentsFlag, coloredFlag);
					printf("The loaded file was released\n");
				}

				getPgmFile(fileName);
				img = readPGM(fileName);
				printf("The file '%s' loaded successfully\n", fileName);

				loadFlag = true;
				segmentsFlag = false;
				coloredFlag = false;
				break;

			case OPTION_2:
				if (!loadFlag)
					printf("You should first read PGM file\n");
				else if (segmentsFlag)
					printf("The segment already founded\n");
				else
				{
					printf("Please enter threshold value:\n");
					scanf("%d", &threshold);
					sizeImgPosList = findAllSegments(img, threshold, &segments);
					printf("All the segments in the file were found\n");
					segmentsFlag = true;
				}
				break;

			case OPTION_3:
				if (!loadFlag)
					printf("You should first read PGM file\n");
				else if (!segmentsFlag)
					printf("You should first find all segments\n");
				else if (coloredFlag)
					printf("Segments already colored\n");
				else
				{
					colorSegmentsImg = colorSegments(segments, sizeImgPosList);
					printf("Color segments was successfully completed\n");
					coloredFlag = true;
				}
				break;

			case OPTION_4:
				if (!coloredFlag)
					printf("You should color segments first\n");
				else
				{
					getCompressFileName(compressFileName);
					max_gray_level = getMaxGrayLevel();
					saveCompressed(compressFileName, colorSegmentsImg, max_gray_level);
					printf("Compress was successfully completed\n");
				}
				break;

			case OPTION_5:
				if (!loadFlag)
					printf("You must load a PGM file before section 5\n");
				else
				{
					getCompressFileName(compressFileName);
					max_gray_level = getMaxGrayLevel();
					saveCompressed(compressFileName, img, max_gray_level);
					printf("Compress was successfully completed\n");
				}
				break;

			case OPTION_6:
				getCompressFileName(compressFileName);
				getConvertedFilename(fileConverted);
				convertCompressedImageToPGM(compressFileName, fileConverted);
				printf("Convert was successfully completed\n");
				break;

			default:
				break;
		}

		getchar();
		printf("Please enter your choice [1-7]:\n");
	}

	// free memory allocations
	freeOldFile(img, colorSegments, segments, sizeImgPosList, loadFlag, segmentsFlag, coloredFlag);
}

void printMenu()
{ // the function prints the menu
	printf("Please enter your choice:\n");
	printf("\t1. Read an image file in PGM format\n");
	printf("\t2. Find all segments\n");
	printf("\t3. Color the segments\n");
	printf("\t4. Save the colored image in a compressed format\n");
	printf("\t5. Compress and save the orignial image in compressed format\n");
	printf("\t6. Convert a compressed image to PGM format\n");
	printf("\t7. Exit\n");
}
void getCompressFileName(char *compressFileName)
{
	printf("Please enter file name:\n");
	scanf("%s", compressFileName);
}
int getMaxGrayLevel()
{
	int maxGrayLevel;
	printf("Please enter max gray level (1-127)\n");
	scanf("%d", &maxGrayLevel);

	return maxGrayLevel;
}
void getPgmFile(char *fileName)
{
	printf("Please enter your PGM file name\n");
	scanf("%s", fileName);
}
void getConvertedFilename(char *fileConverted)
{
	printf("Please enter PGM file name:\n");
	scanf("%s", fileConverted);
}