#include <stdio.h>
#include <stdlib.h>
#include "../io/File.c"


int main() {

    FILE* disk = fopen(vdisk_path, "rb+");

    InitLLFS();

    while (1) {
        int choice;

        scanf("%d", &choice);

        if (choice == 1) {
            create_file(disk, "file1", 0, "/");
			// printf("iteration: 3\n");
			Inode tmp3 = return_inode_struct(disk, ROOT_INODE);
			// printf("tmp3 has %d children \n", tmp3.num_children);
            // print_buffer(dataBitMap, 512);
        }
        else if (choice == 2) {
            create_file(disk, "dir1", 1, "/");
            // print_buffer(dataBitMap, 512);            
        }
        else if (choice == 3) {
            create_file(disk, "dir2", 1, "/");
            // print_buffer(dataBitMap, 512);            
        }
        else if (choice == 4) {
            create_file(disk, "dir3", 1, "/");
            // print_buffer(dataBitMap, 512);            
        }
        else if (choice == 5) {
            create_file(disk, "file2", 0, "/dir2");
            // print_buffer(dataBitMap, 512);            
        }
		else if (choice == 6) {
			create_file(disk, "file3", 0, "/dir3");
		}
        else if (choice == 7) {
            create_file(disk, "dir4", 1, "/dir1");
        }
        else if (choice == 8) {
            create_file(disk, "file4", 0, "/dir1/dir4");
            create_file(disk, "file5", 0, "/dir1/dir4");
        }
        else if (choice == 9) {
            delete_file(disk, "dir3", "/");
        }
        else  if (choice == 10) {
            list_children(disk, ROOT_INODE);
        }
        else {
            break;
        }
    }

	printf("And now here are the children of root(/)\n");
	list_children(disk, ROOT_INODE);

	printf("And now here are the children of dir2 \n");
	list_children(disk, 6);

	printf("And now here are the children of dir3\n");
	list_children(disk, 7);

    printf("And now here are the children of dir4\n");
    list_children(disk, 10);



    



    






    // list_children(disk, ROOT_INODE);
    return 0;
}