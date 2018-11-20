#ifndef structs
#define structs

/////////////////////////////////////////////////
/// Structs
/////////////////////////////////////////////////

typedef struct _grayImage {
	unsigned short rows, cols;
	unsigned char **pixels;
} grayImage;
typedef unsigned short imgPos[2];
typedef struct _treeNode {
	imgPos position;
	struct _treeNodeListCell *next_possible_positions;
} treeNode;
typedef struct _treeNodeListCell {
	treeNode *node;
	struct _treeNodeListCell *next;
} treeNodeListCell;
typedef struct _segment {
	treeNode *root;
} Segment;
typedef struct _segmentListNode {
	Segment seg;
	struct _segmentListNode *next;
} segmentListNode;
typedef struct _segmentList {
	segmentListNode *head, *tail;
} segmentList;

typedef struct _imgPosCell {
	imgPos poition;
	struct _imgPosCell *next, *prev;
} imgPosCell;
typedef struct _imgPosList {
	imgPosCell *head, *tail;
} imgPosList;

#endif