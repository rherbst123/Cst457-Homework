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
#include <pthread.h>

// need a frequency
int frequency[26] = {0};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *processFile(void *fn) {
    // whole process file
    // printf("Processing File: %s\n", (char *)fn);

    // open the file
    FILE *stream = fopen((char *)fn, "rb");

    char c = 0;
    // read the file char by char
    // will stop when C == EOF
    while ((c = fgetc(stream)) != EOF) {
        if (isalpha(c)) {
            // it's an alpha character (a-z)
            c = tolower(c);
            // make darn sure it's a-z
            // stoopid alt char sets
            if (c >= 'a' && c <= 'z') {
                // lock before updating frequency
                pthread_mutex_lock(&lock);
                frequency[c - 'a']++;
                pthread_mutex_unlock(&lock);
            }
        }
    }

    // close the file
    fclose(stream);

    pthread_exit(NULL);
}

void *processDirectory(void *fn) {
    char buffer[256];
    char *directory_path = (char *)fn;
    // get working direct (CWD)
    char *fn_cwd = getcwd(buffer, 256);

    // open directory
    DIR *dir = opendir(directory_path);
    // need a directory entry pointer
    struct dirent *dentry;
    pthread_t threads[1000]; // assuming a maximum of 1000 files in a directory
    int thread_count = 0;

    // loop until dentry is null (no more directory entries)
    while ((dentry = readdir(dir)) != NULL) {
        char file_path[1000] = {0};
        strcat(file_path, directory_path);
        strcat(file_path, "/");
        strcat(file_path, dentry->d_name);

        if (dentry->d_type == DT_DIR) {
            if ((strcmp(dentry->d_name, ".") != 0) && (strcmp(dentry->d_name, "..") != 0)) {
                // create a thread for each subdirectory
                pthread_create(&threads[thread_count], NULL, processDirectory, strdup(file_path));
                thread_count++;
            }
        } else if (dentry->d_type == DT_REG) {
            // create a thread for each file
            pthread_create(&threads[thread_count], NULL, processFile, strdup(file_path));
            thread_count++;
        }
    }
    // close directory
    closedir(dir);

    // wait for all threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_exit(NULL);
}

int main() {
    // Number of runs
    int num_runs = 1000;

    double total_cpu_time = 0.0;

    FILE *outputFile = freopen("System4.txt" , "a" ,stdout);

    for (int run = 0; run < num_runs; ++run) {
        // Start the timer, i.e., record the starting time
        clock_t start = clock();

        char buffer[256];
        // get working direct (CWD)
        char *fn = getcwd(buffer, 256);

        // initialize the mutex
        pthread_mutex_init(&lock, NULL);

        // create a thread for the main directory
        pthread_t main_thread;
        pthread_create(&main_thread, NULL, processDirectory, fn);

        // wait for the main thread to finish
        pthread_join(main_thread, NULL);

        // End the timer, i.e., record the ending time
        clock_t end = clock();

        // destroy the mutex
        pthread_mutex_destroy(&lock);

        // Calculate and print the total time taken
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_cpu_time += cpu_time_used;
    }

    // Calculate and print the average time taken
    double average_cpu_time = total_cpu_time / num_runs;
    printf("Average time taken per run: %f seconds\n", average_cpu_time);

    // output the frequency
    
    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }
    

    return 0;
}