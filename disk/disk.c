#include <stdio.h>
#include "disk.h"

void readBlock(FILE* disk, int blockNum, char* buffer) {
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void writeBlock(FILE* disk, int blockNum, char* data) {
    fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
    fwrite(data, BLOCK_SIZE, 1, disk);
}

// new function, to read file 4 bytes at a time, helps with
// reading info from disk
// void readBlock4Bytes(FILE* disk, int blockNum, char* buffer) {
//     fseek(disk, blockNum * BLOCK_SIZE, SEEK_SET);
//     fread(buffer, BLOCK_SIZE, 4, disk);
// }
