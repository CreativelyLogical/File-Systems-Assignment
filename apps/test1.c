#include <stdio.h>
#include <stdlib.h>
#include "../io/File.c"


int main() {

    FILE* disk = fopen(vdisk_path, "rb+");

    InitLLFS();

    // while (1) {
    //     int choice;

    //     scanf("%d", &choice);

    //     if (choice == 1) {
    //         create_file(disk, "file1", 0, "/");
    //         // print_buffer(dataBitMap, 512);
    //     }
    //     else if (choice == 2) {
    //         create_file(disk, "dir1", 1, "/");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else if (choice == 3) {
    //         create_file(disk, "dir2", 0, "/");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else if (choice == 4) {
    //         create_file(disk, "dir3", 0, "/");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else if (choice == 5) {
    //         create_file(disk, "file2", 1, "/dir2");
    //         // print_buffer(dataBitMap, 512);            
    //     }
    //     else {
    //         break;
    //     }
    // }

    



    






    // list_children(disk, ROOT_INODE);
    return 0;
}