#include "header.h"

/////////////////////////////////////////////////
/// common functions for section 5 and 6
/////////////////////////////////////////////////

unsigned int numOfBits(BYTE num)
{ // the function calculates the number of bits in number
	BYTE res = 0, mask = 0x80;
	while (res < bitsInByte && !(num & mask))
	{
		num <<= 1;
		res++;
	}
	return bitsInByte - res;
}
void setModulos(short *modulo, BYTE *left, BYTE *right, BYTE numOfBitsNeeded)
{ // the function set the new modulos
	if (*modulo == 0)
		*modulo = bitsInByte - numOfBitsNeeded;
	else
		*modulo -= numOfBitsNeeded;

	if (*modulo >= 0)
		*left = numOfBitsNeeded;
	else
		*left = numOfBitsNeeded + *modulo;
	*right = numOfBitsNeeded - *left;
}

/////////////////////////////////////////////////
/// section 5
/////////////////////////////////////////////////

void saveCompressed(char *file_name, grayImage *image, unsigned char max_gray_level)
{ // the function save PGM file in compress binary file
	unsigned char numOfBitsNeeded = numOfBits(max_gray_level);
	unsigned char amountOfExtraBits = bitsInByte - numOfBitsNeeded;
	BYTE temp, tempPrev = 0, bitsMove, bitsLeft, bitsRight;
	short modulo = 0;
	FILE *fp;
	int i, j;

	fp = (FILE *)checkFileOpen(fopen(file_name, "wb"), file_name);
	fwrite(&image->cols, sizeof(unsigned short), 1, fp);
	fwrite(&image->rows, sizeof(unsigned short), 1, fp);
	fwrite(&max_gray_level, sizeof(BYTE), 1, fp);

#ifdef JustForTest
	printf("\nImage before compress:\n");
#endif

	for (i = 0; i < image->rows; i++)
	{
		for (j = 0; j < image->cols; j++)
		{
#ifdef JustForTest
			printf("%4d", image->pixels[i][j]);
#endif
			temp = (image->pixels[i][j] * max_gray_level / 255) << amountOfExtraBits; // remove the unnecessary bits from the left side
			bitsMove = temp >> ((modulo == 0) ? 0 : bitsInByte - modulo); // remove the unnecessary bits from the right side
			tempPrev = tempPrev | bitsMove;
			setModulos(&modulo, &bitsLeft, &bitsRight, numOfBitsNeeded); // update the new modulos

			if (modulo <= 0)
			{
				fwrite(&tempPrev, sizeof(BYTE), 1, fp);
				tempPrev = (temp << bitsLeft);

				if (modulo < 0)
					modulo += bitsInByte;
			}
		}
#ifdef JustForTest
		printf("\n");
#endif
	}
	if (modulo > 0) // writes the last number if there is a modulo
		fwrite(&tempPrev, sizeof(BYTE), 1, fp);

	fclose(fp);
}

/////////////////////////////////////////////////
/// section 6
/////////////////////////////////////////////////

void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name)
{ // convert compress image to PGM file
	int i, j;
	char *string;
	short modulo = 0;
	unsigned short cols, rows;
	BYTE numOfBitsNeeded, bitsLeft, bitsRight, max_gray_level, prevNum, amountOfExtraBits, numTemp, num = 0;

	FILE *compress_file = (FILE *)checkFileOpen(fopen(compressed_file_name, "rb"), compressed_file_name);
	FILE *pgm_file = (FILE *)checkFileOpen(fopen(pgm_file_name, "w"), pgm_file_name);
	fread(&cols, sizeof(unsigned short), 1, compress_file);
	fread(&rows, sizeof(unsigned short), 1, compress_file);
	fread(&max_gray_level, sizeof(BYTE), 1, compress_file);
	string = (char *)checkMemoryAllocations(calloc(getStringLength(cols, rows) + 1, sizeof(char)));

	write_PGM_Header(pgm_file, string, cols, rows, max_gray_level);
	numOfBitsNeeded = numOfBits(max_gray_level);
	amountOfExtraBits = bitsInByte - numOfBitsNeeded;

#ifdef JustForTest
	printf("\nImage after compress:\n");
#endif

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (modulo < numOfBitsNeeded) // check if need to read from file
			{
				prevNum = num;
				fread(&num, sizeof(BYTE), 1, compress_file);
			}

			setModulos(&modulo, &bitsLeft, &bitsRight, numOfBitsNeeded);
			numTemp = ((bitsRight) ? prevNum : num) << (amountOfExtraBits - modulo); // remove the unnecessary bits from the left side
			numTemp >>= amountOfExtraBits; // remove the unnecessary bits from the right side
			numTemp |= num >> (bitsInByte - bitsRight);

			sprintf(string, "%d", numTemp * 255 / max_gray_level);
			fprintf(pgm_file, "%s ", string); // write number to PGM file

#ifdef JustForTest
			printf("%4d", numTemp * 255 / max_gray_level);
#endif

			if (modulo < 0)
				modulo += bitsInByte;
		}
		fprintf(pgm_file, "\n");

#ifdef JustForTest
		printf("\n");
#endif
	}

	free(string);
	fclose(compress_file);
	fclose(pgm_file);
}
void write_PGM_Header(FILE *pgm_file, char *string, unsigned short cols, unsigned short rows, BYTE max_gray_level)
{ // write PGM header to file
	fprintf(pgm_file, "P2\n");
	sprintf(string, "%d ", cols);
	fprintf(pgm_file, "%s", string);
	sprintf(string, "%d", rows);
	fprintf(pgm_file, "%s\n", string);
	sprintf(string, "%d", max_gray_level);
	fprintf(pgm_file, "%s\n", string);
}
int getStringLength(unsigned short cols, unsigned short rows)
{ // return string length
	int max = 3;
	max = MAX(max, getNumberDigits(cols));
	return MAX(max, getNumberDigits(rows));
}
int getNumberDigits(unsigned short num)
{ // return the number of digits in number
	int count = 0;
	while (num / 10)
	{
		num /= 10;
		count++;
	}
	return count;
}