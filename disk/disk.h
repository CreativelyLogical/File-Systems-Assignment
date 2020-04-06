#define BLOCK_SIZE 512
#define NUM_BLOCKS 4096
#define NUM_INODES 128

void readBlock(FILE* disk, int blockNum, char* buffer);
void writeBlock(FILE* disk, int blockNum, char* data);