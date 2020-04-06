#include <stdio.h>
#include <stdlib.h>
#include "../io/File.c"


int main() {

    // FILE* disk = fopen(vdisk_path, "rb+");

    InitLLFS();

    // Touch("file1", "/");

    // Touch("file2", "/");

    // Mkdir("dir1", "/");

    // List("/");

    int i = 0;

    while (1) {
        char command[64];

        fgets(command, 64, stdin);
        printf("length of command is %zu \n", strlen(command));
        printf("the length of quit is %zu \n", strlen("quit\0"));
        command[strlen(command) - 1] = '\0';
        char *token;

        if (strcmp(command, "quit") == 0) {
            break;
        }

        token = strtok(command, " ");

        if (strcmp(token, "touch") == 0) {
            char filename[32];
            char path[32];
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: touch [file name] [path]\n");
                continue;
            }
            strcpy(filename, token);
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: touch [file name] [path]\n");
                continue;
            }
            strcpy(path, token);

            if (Touch(filename, path) == -1) {
                printf("file creation unsuccessful \n");
            } else {
                printf("created file %s in %s \n", filename, path);
            }
        }  
        else if (strcmp(token, "mkdir") == 0) {
            char filename[32];
            char path[32];
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: mkdir [directory name] [path]\n");
                continue;
            }
            strcpy(filename, token);
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: mkdir [directory name] [path]\n");
                continue;
            }
            strcpy(path, token);
            if (Mkdir(filename, path) == -1) {
                printf("directory creation unsuccessful\n");
            }
            else {
                printf("created directory %s in %s \n", filename, path);
            }
        }
        else if (strcmp(token, "ls") == 0) {
            char path[32];
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: ls [path]\n");
                continue;
            }
            strcpy(path, token);
            List(path);
        }
        else if (strcmp(token, "rm") == 0) {
            char filename[32];
            char path[32];
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: rm [file name] [path]\n");
                continue;
            }
            strcpy(filename, token);
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: rm [file name] [path]\n");
                continue;
            }
            strcpy(path, token);
            if (Rm(filename, path) == -1) {
                printf("file deletion unsuccessful\n");
            } else {
                printf("deleted file %s from %s \n", filename, path);
            }
        }
        // while (token != NULL) {
        //     printf("%s \n", token);
        //     token = strtok(NULL, " ");
        // }

        // if (strcmp(token, "touch") == 0) {

            
            
        //     // token = strtok(NULL, " ");
        //     // char *filename;

        //     // strncpy(filename, token, strlen(token));

        //     // token = strtok(NULL, " ");
        //     // char *path;

        //     // strncpy(path, token, strlen(token));

        //     // printf("filename is %s and path is %s \n", filename, path);

        // }
    }

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
    //     else if (choice == 7) {
    //         create_file(disk, "dir4", 1, "/dir1");
    //     }
    //     else if (choice == 8) {
    //         create_file(disk, "file4", 0, "/dir1/dir4");
    //         create_file(disk, "file5", 0, "/dir1/dir4");
    //     }
    //     else if (choice == 9) {
    //         delete_file(disk, "file4", "/dir1/dir4");
    //     }
    //     else  if (choice == 10) {
    //         list_children(disk, 8);
    //     }
    //     else {
    //         break;
    //     }
    // }

	// printf("And now here are the children of root(/)\n");
	// list_children(disk, ROOT_INODE);

	// printf("And now here are the children of dir2 \n");
	// list_children(disk, 6);

	// printf("And now here are the children of dir3\n");
	// list_children(disk, 7);

    // printf("And now here are the children of dir4\n");
    // list_children(disk, 8);



    



    






    // list_children(disk, ROOT_INODE);
    return 0;
}