#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void exploreDirectory(char *path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                //NO FORK NO FORK NO FORK 
                char new_path[1024];
                snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
                exploreDirectory(new_path);
            }
            
        } else if (entry->d_type == DT_REG) {
            printf("File: %s/%s\n", path, entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    exploreDirectory(argv[1]);

    printf("Process exiting: %s\n", argv[1]);

    return 0;

}
