#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h> 



//THIS IS PART 2 WITH FORKING 
void exploreDirectory(char *path) {
    DIR *dir;
    struct dirent *entry;

    //if nothing then show nothing
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }


    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                //FORK FORK FORK FORK FORK FORK FORK 
                pid_t child_pid = fork();
                
                //Error Handling
                if (child_pid == -1) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }

                if (child_pid == 0) {
                    // Child process
                    char new_path[1024];
                    snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
                    exploreDirectory(new_path);
                    //Terminate the child.                   
                    printf("Child process exiting: %s\n", new_path);
                    exit(EXIT_SUCCESS);
                }
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

    // Wait for all child processes to finish
    while (wait(NULL) > 0);

    printf("Parent process exiting: %s\n", argv[1]);

    return 0;

}
