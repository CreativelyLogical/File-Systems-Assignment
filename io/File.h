#include "../disk/disk.h"

#define MAX_FILE_NAME 32
#define MAX_CHILDREN 16
#define ROOT_INODE 3
#define MAX_FILE_SIZE 12 * BLOCK_SIZE

typedef struct {
    /* data */
    int type;
    int size;
    int blockCount;
    int directBlock[12];
    int parentInode;
    int childrenInodes[MAX_CHILDREN];
    int num_children;
    int lastChildIndex;
} Inode;


typedef struct {
    int inode;
    char name[MAX_FILE_NAME]; 
    char content[MAX_FILE_SIZE];
} File;

typedef struct {
    /* data */ 
    int inode;
    char name[MAX_FILE_NAME];
} Directory;



void InitLLFS();

extern char inodeBitMap[NUM_INODES / 8];
extern char dataBitMap[NUM_BLOCKS / 8];

