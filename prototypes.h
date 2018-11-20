#ifndef prototypes
#define prototypes

/////////////////////////////////////////////////
/// Prototypes
/////////////////////////////////////////////////

//// section 1
grayImage *readPGM(char* fname);

//// section 2
void findSingleSegment(grayImage* img, imgPos start, unsigned char threshold);
Segment findSingleSegmentWithBucketMap(grayImage* img, imgPos start, unsigned char threshold, unsigned char** bucket);
void findSingleSegmentRec(treeNode** prevNode, grayImage* img, unsigned char startValue, int rowIndex, int colIndex, unsigned char threshold, unsigned char**bucket);
void findSingleSegmentCheckNeighbors(treeNode** prevNode, grayImage* img, unsigned char startValue, int rowIndex, int colIndex, unsigned char threshold, unsigned char** bucket);
treeNode *createNewTreeNode(int rowIndex, int colIndex);
char checkIfInRange(int rowIndex, int colIndex, unsigned short numRows, unsigned short numCols);
char checkThreshold(unsigned char currValue, unsigned char startValue, unsigned char threshold);
void insertToListCell(treeNode **prevNode, treeNode *newNode);

//// section 3
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList **segments);
imgPosCell* createPosNode(imgPos pos);
void makeEmptySegmentList(segmentList** lst);
void insertDataToEndPosList(imgPosList *lst, imgPos pos);
void insertSingleSegmentToListRec(imgPosList *lst, treeNode *node);
void insertDataToSegmentList(segmentList *lst, Segment seg);
segmentListNode* createSegmentListNode(Segment seg);
short buildSegmentList(segmentList* lst, grayImage *img, unsigned char threshold);
void transferSegmentListToImgPosArray(imgPosList **segments, unsigned int size, segmentList lst);
unsigned char ** buildBucket(unsigned short numRows, unsigned short numCols);
void freeBucket(unsigned char **bucket, unsigned int size);
void freeSegmentList(segmentList segLst);
void freeTreeNode(treeNode* node);

//// section 4
grayImage *colorSegments(imgPosList *segments, unsigned int size);
void colorSegmentsSetPicSize(imgPosList *segments, unsigned int size, unsigned short *numRows, unsigned short *numCols);
grayImage * createGrayImageListFromPosList(imgPosList *segments, unsigned int size);
void freeImgPosArray(imgPosList *segments, unsigned int size);
void freeGrayImage(grayImage *img);

//// section 5
void saveCompressed(char *file_name, grayImage *image, unsigned char max_gray_level);
unsigned int numOfBits(BYTE num);
void setModulos(short *modulo, BYTE *left, BYTE *right, BYTE numOfBitsNeeded);

//// section 6
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name);
void write_PGM_Header(FILE *pgm_file, char *string, unsigned short cols, unsigned short rows, BYTE max_gray_level);
int getNumberDigits(unsigned short num);
int getStringLength(unsigned short cols, unsigned short rows);

#endif