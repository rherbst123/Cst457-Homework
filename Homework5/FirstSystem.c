#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <ctype.h>
#include <dirent.h>

int frequency[26] = {0};

void processFile(const char *fn) { 
  //whole process file
   // printf("Processing File: %s\n",fn);
    
     //open the file
    FILE *stream = fopen(fn, "rb");

  

    int c = 0; 
      //read the file int by int
    //will stop when C == EOF
    while ((c = fgetc(stream)) != EOF) {
        if (isalpha(c)) {
            //it's an alpha character (a-z)
             c = tolower(c);
            //make darn sure it's a-z
            //stoopid alt char sets
            if (c >= 'a' && c <= 'z') {
                //increment frequency
                frequency[c - 'a']++;
            }
        }
    }
//close file
    fclose(stream);
}

void processDirectory(const char *fn) {
    DIR *dir = opendir(fn);

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *dentry;

    while ((dentry = readdir(dir)) != NULL) {
        char buffer[1000] = {0};
        //sprintf(buffer, "%s/%s", fn, dentry->d_name);
        strcat(buffer, fn);
        strcat(buffer, "/");
        strcat(buffer,dentry->d_name);

        if (dentry->d_type == DT_DIR) {
            if (strcmp(dentry->d_name, ".") != 0 && strcmp(dentry->d_name, "..") != 0) {
                processDirectory(buffer);
            }
        } else if (dentry->d_type == DT_REG) {
            processFile(buffer);
        }
    }
    //close directory
    closedir(dir);
}

int main() {
  //need a buffer to read
    char buffer[256];
    //get working direct (CWD)
    char * fn = getcwd(buffer, 256);
   

    int num_runs = 1000;
    double total_cpu_time = 0.0;

    //Use a instead so it writes to it every time instead of redoin it
    FILE *outputFile = freopen("System1.txt", "a", stdout); // Redirect stdout to a file

  
    //timer
    for (int run = 0; run < num_runs; ++run) {
        clock_t start = clock();
        processDirectory(fn);
        clock_t end = clock();

        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_cpu_time += cpu_time_used;
    }

    //Calculate average
    double average_cpu_time = total_cpu_time / num_runs;
    printf("Average time taken per run: %f seconds\n", average_cpu_time, "\n" );


  
    //output the frequency
    for (int i=0; i<26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }
   fclose(outputFile); // Close the file

    return 0;
}


