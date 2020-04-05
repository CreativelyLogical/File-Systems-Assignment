#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../disk/disk.c"
#include "File.h"

#define vdisk_path "../disk/vdisk"

Inode inode[NUM_INODES];
File file[NUM_INODES];
Directory directory[NUM_INODES];
char inodeBitMap[NUM_INODES / 8];
char dataBitMap[NUM_BLOCKS / 8];


// char get_bit(char bitmap[], int bit_num) {
// 	//         get byte num    shift       get val
// 	return (bitmap[bit_num / 8] >> bit_num) & 1;
// }

char get_bit(char bitmap[], int index) {
	return 1 & (bitmap[index/8] >> (index % 8));
}

// void set_bit(char bitmap[], int bit_num) {
// 	// printf("I received number %d \n", bit_num);
// 	// int byte_index = bit_num / 8;
// 	// int bit_index = bit_num % 8;

// 	// bitmap[byte_index] |= 1UL << bit_index;
// 	bitmap[bit_num / 8] |= 1 << (bit_num & 7);
// }

void toggle_bit(char *array, int index) {
	array[index/8] ^= 1 << (index % 8);
}

void set_bit(char bitmap[], int index, char value) {
	if(value != 0 && value != 1) return;
	bitmap[index/8] ^= 1 << (index % 8);
	if(get_bit(bitmap, index) == value) return;
	toggle_bit(bitmap, index);
}

void unset_bit(char bitmap[], int bit_num) {
	// int byte_index = bit_num / 8;
	// int bit_index = bit_num % 8;

	// bitmap[byte_index] &= ~(1UL << bit_index);
	bitmap[bit_num / 8] &= ~(1 << (bit_num & 7));
}

int get_free_inode() {
	for (int i = 0; i < NUM_INODES; i++) {
		if (get_bit(inodeBitMap, i) == 0) {
			set_bit(inodeBitMap, i, 1); // mark it allocated if it's free
			set_bit(dataBitMap, i, 1);
			return i; // return inode index
		}
	}
	return -1;
}

void print_buffer(char buffer[], int size)
{
    printf("%04x: ", 0);
    for (int i = 1; i <= size; i++) {
        printf("%02x ", buffer[i-1]);
        if(i % 8 == 0)
        {
            printf("\n");
            printf("%04x: ", i);
        }
    }
    printf("\n");
}

int get_free_block() {
	// can only get free blocks from blocks 129 and onwards, 
	// because blocks 4 to 132 are for inodes 
	for (int i = 133; i < NUM_BLOCKS; i++) {
		if (get_bit(dataBitMap, i) == 0) {
			printf("block %d is free \n", i);
			set_bit(dataBitMap, i, 1);
			return i;
		}
	}

	return -1;
}

Inode return_inode_struct(FILE *disk, int inodeNum) {
	char buffer[512];

	// read provided block into buffer
	readBlock(disk, inodeNum, buffer);

	// print_buffer(buffer, 512);

	// copy the buffer onto the struct
	Inode tmp;
	memcpy((char *) &tmp, buffer, sizeof(Inode));

	return tmp;

}

void list_children(FILE *disk, int inodeNum) {
	// printf("im here yalll\n");
	Inode curInode = return_inode_struct(disk, inodeNum);

	for (int i = 0; i < curInode.num_children; i++) {
		int childInode = curInode.childrenInodes[i];
		Inode childInodeStruct = return_inode_struct(disk, childInode);
		char fileBuffer[512];
		readBlock(disk, childInodeStruct.directBlock[0], fileBuffer);

		File curFile;
		memcpy((char*) &curFile, fileBuffer, 68);
		printf("the name of the file is %s \n", curFile.name);
	}
}

void list_child_inodes(FILE *disk, int inodeNum) {
	Inode curInode = return_inode_struct(disk, inodeNum);

	for (int i = 0; i < curInode.num_children; i++) {
		int childInode = curInode.childrenInodes[i];
		Inode childInodeStruct = return_inode_struct(disk, childInode);
		printf("child %d is %d and it starts at block %d\n", i, childInode, childInodeStruct.directBlock[0]);
	}
}

int find_parent_file_path(FILE *disk, char *path) {
	char *token;


	token = strtok(path, "/");

	Inode curInode = return_inode_struct(disk, ROOT_INODE);

	char *root = "/"; // we'll start our search at root


	// Note, the only things the tokens SHOULD be are directories
	while (token != NULL) {
		printf("%s \n", token);
		
		// iterate through the inode of parent directory
		for (int i = 0; i < curInode.num_children; i++) {
			int childInode = curInode.childrenInodes[i];
			// printf("the %dth child of the root has inode %d\n", i, curInode.childrenInodes[i]);
			Inode childInodeStruct = return_inode_struct(disk, childInode);
			// printf("File with inode %d starts at block %d \n", childInode, childInodeStruct.directBlock[0]);

			if (childInodeStruct.type == 0) {
				continue;
			}
			char fileBuffer[512];
			// read the block where the file is
			readBlock(disk, childInodeStruct.directBlock[0], fileBuffer); 

			// copy from the file to a File struct

			Directory tmpDir;
			memcpy((char*) &tmpDir, fileBuffer, 68);
			
			// printf("The name of the directory is %s \n", tmpDir.name);

			// printf("token is %s and the file name is %s \n", token, file[childInode].name);
			if (strcmp(token, tmpDir.name) == 0) {
				// printf("match, returning the inode now...\n");
				return childInode;
			} else {
				continue;
			}

		}

		token = strtok(NULL, "/");
	}

	return -1;
}



int create_file(FILE *disk, char *name, int type, char *path) {
	// printf("test: the name of the file to create is %s \n", name);

	
	if (strcmp(path, "/") == 0) {
		
		printf("iteration: 1\n");
		Inode root = return_inode_struct(disk, 3);
		
		printf("the root currently has %d children \n", root.num_children);

		for (int i = 0; i < root.num_children; i++) {
			printf("howdy\n");
			int childInode = root.childrenInodes[i];
			// printf("hi\n");
			Inode childInodeStruct = return_inode_struct(disk, childInode);
			char fileBuffer[512];
			readBlock(disk, childInodeStruct.directBlock[0], fileBuffer); 
			Directory tmpDir;
			memcpy((char*) &tmpDir, fileBuffer, 68);
			// printf("name is %s and tmpDir.name is %s \n", name, tmpDir.name);
			if (strcmp(name, tmpDir.name) == 0) {
				printf("file already exists in this directory\n");
				return -1;
			}
		}
	}

	int inodeNum = get_free_inode();



	// set_bit(dataBitMap, inodeNum); // the block containing the inode will also be allocated now
	int parentInode;

	int blockNum = get_free_block();

	printf("inode %d's block number is %d \n", inodeNum, blockNum);

	// if (blockNum == -1) {
	// 	printf("what's going on?\n");
	// }

	// set_bit(dataBitMap, blockNum); // mark the blockNum as used as well

	// check if path is simply root
	if (strcmp(path, "/") == 0) {
		parentInode = ROOT_INODE;
	} else {
		// can't pass in path to find_parent_file_path since path is read-only memory, hence strtok trying to modify
		// read-only memory would lead to a segmentation fault

		// path_to_tokenize is not read-only memory, it can be modified, hence we can pass it to strtok without problems
		char path_to_tokenize[strlen(path)]; 

		strncpy(path_to_tokenize, path, strlen(path));

		path_to_tokenize[strlen(path)] = '\0';

		// printf("path_to_tokenize is %s \n", path_to_tokenize);

		parentInode = find_parent_file_path(disk, path_to_tokenize);

		// printf("parentInode now is %d and its name is %s \n", parentInode, directory[parentInode].name);

	}

	// initialize inode
	inode[inodeNum].type = type;
	inode[inodeNum].size = 1;
	inode[inodeNum].blockCount = 1;
	inode[inodeNum].directBlock[0] = blockNum;
	inode[inodeNum].parentInode = parentInode;

	// set this inode to be a child of the parent inode (IF parent is a directory)


	
	if (type == 1) { // file is a directory
		inode[inodeNum].num_children = 0; // so far the directory has 0 children
		inode[inodeNum].lastChildIndex = 0;
		directory[inodeNum].inode = inodeNum;
		strcpy(directory[inodeNum].name, name);
		
		set_bit(dataBitMap, blockNum + 1, 1); // just giving it some space
		set_bit(dataBitMap, blockNum + 2, 1);
	}  else { // file is a regular file

		// because a file is 6212 bytes, it takes up 13 blocks, hence mark the next
		// 12 blocks as allocated
		for (int i = 1; i <= 12; i++) {
			set_bit(dataBitMap, blockNum + i, 1);
			// printf("blocks %d which is byte %d\n", blockNum + i, (blockNum + i) / 8);
		}

		if (get_bit(dataBitMap, blockNum + 1) == 1) {
			printf("this was set \n");
		}
		// create the file
		file[inodeNum].inode = inodeNum;
		// give the name to the file
		strcpy(file[inodeNum].name, name); 
	}

	// store inode into disk
	writeBlock(disk, inodeNum, (char*) (inode + inodeNum));

	


	// store file into disk
	if (type == 1) {
		writeBlock(disk, blockNum, (char*) (directory + inodeNum)); // if directory
	} else {
		writeBlock(disk, blockNum, (char*) (file + inodeNum)); // if regular file
	}
	

	// printf("And now let's try to read the block that we just wrote to\n");

	char buffer[512];

	// copying the parent inode from the buffer to the struct because we'll be modifying it:
	// ... adding the current inode to as the parent's child, incrementing the parent's num_children etc..

	readBlock(disk, parentInode, buffer);


	// printf("copying from buffer onto struct...\n");

	memcpy((char*) (inode + parentInode), buffer, sizeof(Inode));


	printf("parent before had %d chilren \n", inode[parentInode].num_children);

	// printf("now, modify the struct's contents...\n");
	inode[parentInode].childrenInodes[inode[parentInode].lastChildIndex] = inodeNum; // add the inode of the file to the parent's children inodes
	inode[parentInode].lastChildIndex += 1; 
	inode[parentInode].num_children += 1;


	
	writeBlock(disk, parentInode, (char*) (inode + parentInode)); // update the parent inode


	printf("iteration: 2\n");
	Inode tmp2 = return_inode_struct(disk, parentInode);


	// update the bitmaps
	writeBlock(disk, 1, inodeBitMap);
	writeBlock(disk, 2, dataBitMap);

	if (get_bit(dataBitMap, 144) == 0) {
		printf("WTF\n");
	}

	printf("parent now has %d children \n", tmp2.num_children);

	return -1;
}


void InitLLFS() {
	FILE* disk = fopen(vdisk_path, "wb"); // ab creates file if none exists
	char* init = calloc(BLOCK_SIZE * NUM_BLOCKS, 1);
	fwrite(init, BLOCK_SIZE * NUM_BLOCKS, 1, disk);
	free(init);
	fclose(disk);

	disk = fopen(vdisk_path, "rb+");


	// Initialize Block 0
	char* buffer;
    
	buffer = (char *) malloc(BLOCK_SIZE);
	

	int magic = 42; // magic number up to us
	int blocks = NUM_BLOCKS;
	int inodes = NUM_INODES;
    
	memcpy(buffer, &magic, sizeof(int));
	memcpy(buffer + sizeof(int) * 1, &blocks, sizeof(int));
	memcpy(buffer + sizeof(int) * 2, &inodes, sizeof(int));
    
	writeBlock(disk, 0, buffer);

	// initialize the bitmaps, setting all unused bits to 0 and used bits to 1

	// set all inode bitmaps to 0
	for (int i = 0; i < NUM_INODES; i++) {
		unset_bit(inodeBitMap, i);
	}

	// we won't be using the first 3 inodes, since the first inode will start at 3 (ROOT_INODE)
	set_bit(inodeBitMap, 0, 1);
	set_bit(inodeBitMap, 1, 1);
	set_bit(inodeBitMap, 2, 1);


	set_bit(dataBitMap, 0, 1); // superblock
	set_bit(dataBitMap, 1, 1); // inode bitmap
	set_bit(dataBitMap, 2, 1); // dataBlock bitmap
	
	// set the rest of the dataBlock bitmap to 0
	for (int i = 3; i < NUM_BLOCKS; i++) {
		unset_bit(dataBitMap, i);
	}


	char root = '/';


	// Initialize root directory
	int rootInode = ROOT_INODE;
	set_bit(dataBitMap, ROOT_INODE, 1); // mark third block as allocated
	set_bit(inodeBitMap, ROOT_INODE, 1);


	int rootDirBlock = 4;
	set_bit(dataBitMap, rootDirBlock, 1); 

	// so, so far, we have

	/* block    allocated for
		0	-->	   superblock
		1	-->	   inode bitmap
		2	-->	   data bitmap
		3	-->    root inode
		4	-->    root directory content 		
	*/

	printf("the root directory inode is %d \n", rootInode);
	printf("the root directory block is %d \n", rootDirBlock);

	// char* inodeBuf = (char *) malloc(BLOCK_SIZE);

	inode[rootInode].type = 1; // 1 for directory, 0 for regular file
	inode[rootInode].size = 1;
	inode[rootInode].blockCount = 1;
	inode[rootInode].directBlock[0] = rootDirBlock;
	inode[rootInode].parentInode = 128;
	inode[rootInode].num_children = 0;
	inode[rootInode].lastChildIndex = 0;


	// writeBlock(disk, rootInode, inodeBuf);
	// printf("%s \n", (char*) &inode[rootInode]);
	writeBlock(disk, rootInode, (char*) (inode + rootInode));


	writeBlock(disk, 1, inodeBitMap);
	writeBlock(disk, 2, dataBitMap);




    // while (1) {
    //     int choice;

    //     scanf("%d", &choice);

    //     if (choice == 1) {
    //         create_file(disk, "file1", 0, "/");
	// 		// printf("iteration: 3\n");
	// 		Inode tmp3 = return_inode_struct(disk, ROOT_INODE);
	// 		// printf("tmp3 has %d children \n", tmp3.num_children);
    //         // print_buffer(dataBitMap, 512);
    //     }
    //     else if (choice == 2) {
    //         create_file(disk, "dir1", 1, "/");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else if (choice == 3) {
    //         create_file(disk, "dir2", 1, "/");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else if (choice == 4) {
    //         create_file(disk, "dir3", 1, "/");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else if (choice == 5) {
    //         create_file(disk, "file2", 0, "/dir2");
    //         // print_buffer(dataBitMap, 512);            
    //     }
	// 	else if (choice == 6) {
	// 		create_file(disk, "file3", 0, "/dir3");
	// 	}
    //     else {
    //         break;
    //     }
    // }

	printf("And now here are the children of root(/)\n");
	list_children(disk, ROOT_INODE);

	printf("And now here are the children of dir2 \n");
	list_children(disk, 6);

	printf("And now here are the children of dir3\n");
	list_children(disk, 7);
	// list_child_inodes(disk, ROOT_INODE);
	


	// create_file(disk, "file1", 0, "/");

	// create_file(disk, "dir1", 1, "/");

	// create_file(disk, "dir2", 1, "/");

	// create_file(disk, "dir3", 1, "/");
	// create_file(disk, "dir2", 1, "/dir1");

	// create_file(disk, "file2", 0, "/dir2");

	// printf("Finally, let's list the children of the root directory\n");

	// list_children(disk, ROOT_INODE);

	// Inode tmp1 = return_inode_struct(disk, rootInode);

	// printf("tmp1.num_children is %d \n", tmp1.num_children);


	// Inode tmp2 = return_inode_struct(disk, rootInode);


	// printf("tmp2.num_children is %d \n", tmp2.num_children);

	// Inode tmp3 = return_inode_struct(disk, rootInode);


	// printf("tmp3.num_children is %d \n", tmp3.num_children);

	// print_buffer(dataBitMap, 512);
	// printf("dir 2 has %d children \n", inode[6].num_children);

	free(buffer);
	fclose(disk);
}