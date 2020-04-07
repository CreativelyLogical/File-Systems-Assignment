#include <stdio.h>
#include <stdlib.h>
#include "../io/File.c"


int main() {

    int i = 0;

    while (1) {
        char command[256];

        fgets(command, 64, stdin);
        command[strlen(command) - 1] = '\0';
        char *token;

        if (strcmp(command, "quit") == 0) {
            break;
        }

        token = strtok(command, " ");
        if (strcmp(token, "init") == 0) {
            InitLLFS();
        }
        else if (strcmp(token, "touch") == 0) {
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
                // printf("created file %s in %s \n", filename, path);
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
                // printf("created directory %s in %s \n", filename, path);
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
                // printf("deleted file %s from %s \n", filename, path);
            }
        }
        else if (strcmp(token, "append") == 0) {
            char filename[32];
            char path[32];
            char buffer[192];

            // printf("buffer[0] is %d \n", buffer[0]);
            buffer[0] = '\0';

            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: append [file name] [path] [buffer/content]\n");
                continue;
            }
            strcpy(filename, token);
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: append [file name] [path] [buffer/content]\n");
                continue;
            }
            strcpy(path, token);
            token = strtok(NULL, "");
            // strtok(NULL, " ");
            // strtok(NULL, " ");
            while (token != NULL) {
                strcat(buffer, token);
                token = strtok(NULL, "");
            }
            Write(filename, path, buffer);
        }
        else if (strcmp(token, "cat") == 0) {
            char filename[32];
            char path[32];
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: cat [file name] [path]\n");
                continue;
            }
            strcpy(filename, token);
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("usage: cat [file name] [path]\n");
                continue;
            }
            strcpy(path, token);
            Read(filename, path);
        }
    }

    return 0;
}