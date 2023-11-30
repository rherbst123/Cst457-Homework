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

void* processFile(void *fn);
void* processDirectory(void *letter);

void* processFile(void *fn) {
    const char *file_path = (const char *)fn;
    //open the file
    FILE *stream = fopen(file_path, "rb");



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
                //No locks necessary
                
                frequency[c - 'a']++;
            
            }
        }
    }

    fclose(stream);
    
    pthread_exit(NULL);
}

void* processDirectory(void *letter) {
    char letter_char = *((char *)letter);
    char buffer[256];
    char *fn = getcwd(buffer, 256);

    DIR *dir = opendir(fn);

  

    // need a directory entry pointer
    struct dirent *dentry;
    pthread_t threads[1000]; // assuming a maximum of 1000 files in a directory
    int thread_count = 0;

    while ((dentry = readdir(dir)) != NULL && thread_count < 1000) {
        char file_letter = tolower(dentry->d_name[0]);
        if (isalpha(file_letter) && file_letter == letter_char) {
            char file_path[1000] = {0};
            snprintf(file_path, sizeof(file_path), "%s/%s", fn, dentry->d_name);
            pthread_create(&threads[thread_count], NULL, processFile, strdup(file_path));
            thread_count++;
        }
    }

    closedir(dir);

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_exit(NULL);
}

int main() {
    int num_runs = 1000;
    double total_cpu_time = 0.0;

    FILE *outputFile = freopen("System3.txt" , "a" ,stdout);

    for (int run = 0; run < num_runs; ++run) {
        clock_t start = clock();

        pthread_t threads[26];
        char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

        for (int i = 0; i < 26; i++) {
            pthread_create(&threads[i], NULL, processDirectory, &letters[i]);
        }

        for (int i = 0; i < 26; i++) {
            pthread_join(threads[i], NULL);
        }

        clock_t end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_cpu_time += cpu_time_used;
    }

    double average_cpu_time = total_cpu_time / num_runs;
    printf("Average time taken per run: %f seconds\n", average_cpu_time);

    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }

    return 0;
}
