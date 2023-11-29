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

void processFile(const char *fn) { // Corrected parameter type and added const
    FILE *stream = fopen(fn, "rb");

    if (stream == NULL) {
        perror("Error opening file");
        return;
    }

    int c = 0; // Changed type to int for EOF handling
    while ((c = fgetc(stream)) != EOF) {
        if (isalpha(c)) {
            c = tolower(c);
            if (c >= 'a' && c <= 'z') {
                frequency[c - 'a']++;
            }
        }
    }

    fclose(stream);
}

void processDirectory(const char *fn) { // Corrected parameter type and added const
    DIR *dir = opendir(fn);

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *dentry;

    while ((dentry = readdir(dir)) != NULL) {
        char buffer[1000] = {0};
        sprintf(buffer, "%s/%s", fn, dentry->d_name);

        if (dentry->d_type == DT_DIR) {
            if (strcmp(dentry->d_name, ".") != 0 && strcmp(dentry->d_name, "..") != 0) {
                processDirectory(buffer);
            }
        } else if (dentry->d_type == DT_REG) {
            processFile(buffer);
        }
    }

    closedir(dir);
}

int main() {
    char buffer[256];
    char *fn = getcwd(buffer, 256);

    if (fn == NULL) {
        perror("Error getting current directory");
        return 1;
    }

    int num_runs = 1000;
    double total_cpu_time = 0.0;

    FILE *outputFile = freopen("System1.txt", "a", stdout); // Redirect stdout to a file

    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    for (int run = 0; run < num_runs; ++run) {
        clock_t start = clock();
        processDirectory(fn);
        clock_t end = clock();

        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_cpu_time += cpu_time_used;
    }

    double average_cpu_time = total_cpu_time / num_runs;
    printf("Average time taken per run: %f seconds\n", average_cpu_time, "\n" );


    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }

   fclose(outputFile); // Close the file

    return 0;
}
