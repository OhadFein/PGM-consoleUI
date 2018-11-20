#include "header.h"

/////////////////////////////////////////////////
/// section 1
/////////////////////////////////////////////////

grayImage *readPGM(char* fname)
{ // read PGM image file into grayImage struct
	int max, i, j;
	FILE *fp;
	char pgmType[3];
	grayImage *pic = (grayImage*)checkMemoryAllocations(malloc(sizeof(grayImage)));

	fp = checkFileOpen(fopen(fname, "r"), fname);
	fgets(pgmType, sizeof(pgmType), fp);

	if (strcmp(pgmType, "P2"))
	{
		printf("Wrong file type\n");
		exit(1);
	}

	fscanf(fp, "%hu %hu %d", &pic->cols, &pic->rows, &max);
	pic->pixels = (unsigned char **)checkMemoryAllocations(calloc(pic->rows, sizeof(unsigned char *)));

	for (i = 0; i < pic->rows; i++)
		pic->pixels[i] = (unsigned char *)checkMemoryAllocations(calloc(pic->cols, sizeof(unsigned char)));

	//insert values the matrix
	for (i = 0; i < pic->rows; i++)
		for (j = 0; j < pic->cols; j++)
			fscanf(fp, "%hhu ", &pic->pixels[i][j]);

	fclose(fp);

	return pic;
}

/////////////////////////////////////////////////
/// section 2
/////////////////////////////////////////////////

void findSingleSegment(grayImage *img, imgPos start, unsigned char threshold)
{ // call to recursive function that find single segment in grayImage
	unsigned char **bucket = buildBucket(img->rows, img->cols);
	findSingleSegmentWithBucketMap(img, start, threshold, bucket);
}
Segment findSingleSegmentWithBucketMap(grayImage *img, imgPos start, unsigned char threshold, unsigned char **bucket)
{ // call to recursive function that find single segment in grayImage from the segments that not been found yet
	Segment segment;
	segment.root = NULL;
	findSingleSegmentRec(&segment.root, img, img->pixels[start[0]][start[1]], start[0], start[1], threshold, bucket);
	return segment;
}
void findSingleSegmentRec(treeNode** prevNode, grayImage *img, unsigned char startValue, int rowIndex, int colIndex, unsigned char threshold, unsigned char **bucket)
{ // find single segment in recursive way in grayImage
	treeNode *newNode;
	unsigned char currValue;
	if (checkIfInRange(rowIndex, colIndex, img->rows, img->cols))
	{
		currValue = img->pixels[rowIndex][colIndex];
		// checks if the current position is already found in segment and in the threshold's range
		if (bucket[rowIndex][colIndex] == PIXEL_NOT_FOUND_IN_BUCKET_MAP && checkThreshold(currValue, startValue, threshold))
		{
			bucket[rowIndex][colIndex] = PIXEL_FOUND_IN_BUCKET_MAP;		// update the bucket
			newNode = createNewTreeNode(rowIndex, colIndex);			// create new node
			insertToListCell(prevNode, newNode);						// insert the new node
			findSingleSegmentCheckNeighbors(&newNode, img, startValue, rowIndex, colIndex, threshold, bucket);	// check neighbors
		}
	}
}
treeNode *createNewTreeNode(int rowIndex, int colIndex)
{ // create new treeNode
	treeNode *newNode;
	newNode = (treeNode *)checkMemoryAllocations(malloc(sizeof(treeNode)));
	newNode->position[0] = rowIndex;
	newNode->position[1] = colIndex;
	newNode->next_possible_positions = NULL;
	return newNode;
}
void insertToListCell(treeNode **prevNode, treeNode *newNode)
{ // insert data to listCell
	treeNodeListCell* newListCell;
	if (*prevNode != NULL)
	{
		newListCell = (treeNodeListCell *)checkMemoryAllocations(malloc(sizeof(treeNodeListCell)));
		newListCell->node = newNode;
		newListCell->next = NULL;

		if ((*prevNode)->next_possible_positions != NULL)
			newListCell->next = (*prevNode)->next_possible_positions;
		(*prevNode)->next_possible_positions = newListCell;
	}
	else
		*prevNode = newNode;
}
void findSingleSegmentCheckNeighbors(treeNode** prevNode, grayImage *img, unsigned char startValue, int rowIndex, int colIndex, unsigned char threshold, unsigned char **bucket)
{
	int i, numberOfNeighbors = 8;
	const int rowSteps[] = { -1,-1,-1,0,0,1,1,1 };
	const int colSteps[] = { -1,0,1,-1,1,-1,0,1 };

	// call to the neighbors in the matrix in this order:
	// 1 2 3
	// 4 . 5
	// 6 7 8

	for (i = 0; i < numberOfNeighbors; i++)
		findSingleSegmentRec(prevNode, img, startValue, rowIndex + rowSteps[i], colIndex + colSteps[i], threshold, bucket);	// 1
}
char checkThreshold(unsigned char currValue, unsigned char startValue, unsigned char threshold)
{ // checks threshold
	return (abs(currValue - startValue) <= threshold);
}
char checkIfInRange(int rowIndex, int colIndex, unsigned short numRows, unsigned short numCols)
{ // checks if the indexes are in the range of the picture
	return (rowIndex >= 0 && rowIndex < numRows && colIndex >= 0 && colIndex < numCols);
}

/////////////////////////////////////////////////
/// section 3
/////////////////////////////////////////////////

int findAllSegments(grayImage *img, unsigned char threshold, imgPosList **segments)
{ // find all segments in grayImage
	segmentList segLst;
	short size = buildSegmentList(&segLst, img, threshold); // finds segments in grayImage and insert to segment list
	transferSegmentListToImgPosArray(segments, size, segLst); // build imgPos array and insert the segments to the list
	freeSegmentList(segLst); // free segment list
	return size;
}

// build functions
unsigned char ** buildBucket(unsigned short numRows, unsigned short numCols)
{ // build new bucket in grayImage size
	int i;
	unsigned char **bucket = (unsigned char **)checkMemoryAllocations(calloc(numRows, sizeof(unsigned char *)));
	for (i = 0; i < numRows; i++)
		bucket[i] = (unsigned char *)checkMemoryAllocations(calloc(numCols, sizeof(unsigned char)));
	return bucket;
}
short buildSegmentList(segmentList* lst, grayImage *img, unsigned char threshold)
{ // finds the segments in grayImage and insert to segment list
	int i, j;
	imgPos pos;
	Segment seg;
	short size = 0;
	unsigned char **bucket = buildBucket(img->rows, img->cols);
	makeEmptySegmentList(&lst);

	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
		{
			if (bucket[i][j] == PIXEL_NOT_FOUND_IN_BUCKET_MAP)
			{
				pos[0] = i;
				pos[1] = j;
				seg = findSingleSegmentWithBucketMap(img, pos, threshold, bucket); // find single segment
				insertDataToSegmentList(lst, seg); // insert single segment to segment list
				size++;
			}
		}
	}

	freeBucket(bucket, img->rows); // free bucket
	return size;
}
void transferSegmentListToImgPosArray(imgPosList **segments, unsigned int size, segmentList lst)
{ // transfer segment list into imgPos array
	short index = 0;
	treeNode *ptrTreeNode;
	segmentListNode *ptrSegmentList = lst.head;
	imgPosList *segmentArr = (imgPosList *)checkMemoryAllocations(calloc(size, sizeof(imgPosList))); // create array
	while (ptrSegmentList)
	{
		ptrTreeNode = ptrSegmentList->seg.root;
		insertSingleSegmentToListRec(&segmentArr[index], ptrTreeNode); // insert single segment to list in recursive
		ptrSegmentList = ptrSegmentList->next;
		index++;
	}
	*segments = segmentArr;
}

////// list functions - section 3 //////

void makeEmptySegmentList(segmentList** lst)
{ // make an empty segment list
	(*lst)->head = (*lst)->tail = NULL;
}
void insertSingleSegmentToListRec(imgPosList *lst, treeNode *node)
{ // insert single segment (tree) for imgPos list in recursive way
	treeNodeListCell *ptr;
	if (lst == NULL || node == NULL)
		return;

	ptr = node->next_possible_positions;
	insertDataToEndPosList(lst, node->position);
	while (ptr)
	{
		insertSingleSegmentToListRec(lst, ptr->node);
		ptr = ptr->next;
	}
}
void insertDataToEndPosList(imgPosList *lst, imgPos pos)
{ // insert data to end pos list
	imgPosCell *newTail;
	newTail = createPosNode(pos);
	if (lst->head)
	{
		newTail->prev = lst->tail;
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
	else
		lst->head = lst->tail = newTail;
}
void insertDataToSegmentList(segmentList *lst, Segment seg)
{ // insert data to end segment list
	segmentListNode *newTail;
	newTail = createSegmentListNode(seg);
	if (lst->head)
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
	else
		lst->head = lst->tail = newTail;
}
segmentListNode* createSegmentListNode(Segment seg)
{ // create segmentListNode
	segmentListNode *res = (segmentListNode *)checkMemoryAllocations(malloc(sizeof(segmentListNode)));
	res->seg = seg;
	res->next = NULL;
	return res;
}
imgPosCell* createPosNode(imgPos pos)
{ // create node of imgPos
	imgPosCell *res;
	res = (imgPosCell *)checkMemoryAllocations(malloc(sizeof(imgPosCell)));
	res->poition[0] = pos[0];
	res->poition[1] = pos[1];
	res->next = NULL;
	res->prev = NULL;
	return res;
}

////// free functions - section 3 //////

void freeBucket(unsigned char **bucket, unsigned int size)
{ // free bucket
	unsigned int i;
	for (i = 0; i < size; i++)
		free(bucket[i]);
	free(bucket);
}
void freeSegmentList(segmentList segLst)
{ // free segmentList
	segmentListNode* ptr = segLst.head, *next;
	while (ptr)
	{
		next = ptr->next;
		freeTreeNode(ptr->seg.root);
		free(ptr);
		ptr = next;
	}
}
void freeTreeNode(treeNode* node)
{ // free treeNode
	treeNodeListCell *ptr, *ptrTemp;
	if (!node)
		return;
	ptr = node->next_possible_positions;
	while (ptr)
	{
		ptrTemp = ptr;
		freeTreeNode(ptr->node);
		ptr = ptr->next;
		free(ptrTemp);
	}
	free(node);
}
void freeImgPosArray(imgPosList *segments, unsigned int size)
{ // free imgPosArray
	unsigned int i;
	imgPosCell *ptr, *ptrTemp;
	for (i = 0; i < size; i++)
	{
		ptr = ((segments)+i)->head;
		while (ptr)
		{
			ptrTemp = ptr;
			ptr = ptr->next;
			free(ptrTemp);
		}
	}
	free(segments);
}


/////////////////////////////////////////////////
/// section 4
/////////////////////////////////////////////////

grayImage *colorSegments(imgPosList *segments, unsigned int size)
{ // create grayImage and return the image after colored the segments
	unsigned int i;
	imgPosCell *ptrPos;
	grayImage *res = createGrayImageListFromPosList(segments, size);
	for (i = 0; i < size; i++)
	{
		ptrPos = (segments + i)->head;
		while (ptrPos)
		{
			// puts zero if there is only 1 segment, otherwise puts according to the formula
			res->pixels[ptrPos->poition[0]][ptrPos->poition[1]] = (size == 1) ? (0) : ((255 * i) / (size - 1));
			ptrPos = ptrPos->next;
		}
	}

	return res;
}
grayImage * createGrayImageListFromPosList(imgPosList *segments, unsigned int size)
{ // create grayImage list from imgPos list
	int i;
	grayImage *img = (grayImage *)checkMemoryAllocations(malloc(sizeof(grayImage)));
	colorSegmentsSetPicSize(segments, size, &img->rows, &img->cols);
	img->pixels = (unsigned char **)checkMemoryAllocations(calloc(img->rows, sizeof(unsigned char *)));

	for (i = 0; i < img->rows; i++)
		img->pixels[i] = (unsigned char *)checkMemoryAllocations(calloc(img->cols, sizeof(unsigned char)));
	return img;
}
void colorSegmentsSetPicSize(imgPosList *segments, unsigned int size, unsigned short *numRows, unsigned short *numCols)
{ // set picture size of grayImage
	unsigned int i;
	imgPosCell *ptrPos;
	*numRows = *numCols = 0;
	for (i = 0; i < size; i++) // find number of rows and cols
	{
		ptrPos = (segments + i)->head;
		while (ptrPos)
		{
			if (ptrPos->poition[0] > *numRows)
				*numRows = ptrPos->poition[0];
			if (ptrPos->poition[1] > *numCols)
				*numCols = ptrPos->poition[1];
			ptrPos = ptrPos->next;
		}
	}
	(*numRows)++;
	(*numCols)++;
}
void freeGrayImage(grayImage *img)
{ // free grayImage
	int i;

	for (i = 0; i < img->rows; i++)
		free(img->pixels[i]);

	free(img->pixels);
	free(img);
}