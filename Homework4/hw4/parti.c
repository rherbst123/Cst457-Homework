#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int match_option(char* filepath, char* option, struct stat* file_stat);
void search(char* directory, char** options, int depth, int recursive);
int match_options(char* filepath, char** options, struct stat* file_stat);

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <directory> [options...] [-R]\n", argv[0]);
    return 1;
  }

  char* directory = argv[1;
  char** options = argv + 2;
  int recursive = 0;

  // Check for the -r option
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-r") == 0) {
      recursive = 1;
      break;
    }
  }

  search(directory, options, 0, recursive);

  return 0;
}

//Error stuff 
void search(char* directory, char** options, int depth, int recursive) {
  DIR* dir = opendir(directory);
  if (dir == NULL) {
    fprintf(stderr, "Error opening directory: %s\n", directory);
    return;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char* filepath = malloc(strlen(directory) + 1 + strlen(entry->d_name) + 1);
    sprintf(filepath, "%s/%s", directory, entry->d_name);

    struct stat file_stat;
    if (stat(filepath, &file_stat) != 0) {
      fprintf(stderr, "Error getting file information: %s\n", filepath);
      free(filepath);
      continue;
    }
  //Format output
    if (match_options(filepath, options, &file_stat)) {
      printf("Name: %s, UID: %d, Size(how man bytes): %zu, Depth: %d\n",
             entry->d_name, file_stat.st_uid, file_stat.st_size, depth);
    }

    if (recursive && entry->d_type == DT_DIR) {
      // Recursively search subdirectories.
      search(filepath, options, depth + 1, recursive);
    }

    free(filepath);
  }

  closedir(dir);
}

// Check if the given file matches all the specified options.
int match_options(char* filepath, char** options, struct stat* file_stat) {
  for (int i = 0; options[i] != NULL; i++) {
    if (options[i][0] == '-') {
      if (!match_option(filepath, options[i], file_stat)) {
        return 0;
      }
    }
  }

  return 1;
}

// Check if the given file matches a single option.
int match_option(char* filepath, char* option, struct stat* file_stat) {
  // NAME FUNCTION
  if (strcmp(option, "-name") == 0) {
    char* filename = strrchr(filepath, '/');
    if (filename == NULL) {
      filename = filepath; // No path separator found, use the whole path
    // Move past the last path separator
    }
    return strcmp(filename, option) == 0;
    // UID
    //WORKS AND TESTED
  } else if (strcmp(option, "-uid") == 0) {
    uid_t target_uid = atoi(option + 5); // Skip the "-uid" part
    return file_stat->st_uid == target_uid;
    // TYPES
    //DOESNT WORK YET??
  } else if (strcmp(option, "-type") == 0) {
    // for files
    if (option[1] == 'f') {
      return S_ISREG(file_stat->st_mode);
      // for directories
    } else if (option[1] == 'd') {
      return S_ISDIR(file_stat->st_mode);
    } else {
      return 0;
    }
    // LESS THAN AND GREATER THAN
    //THIS BE THE LESS THAN
    //just do this twice and change the allagator
    //WORKS AND TESTED
} else if (strncmp(option, "-lt", 3) == 0) {
    int threshold = atoi(option + 3); // Skip the "-lt" part
    return file_stat->st_size < threshold;
    //THIS BE THE GREATER THAN
} else if (strncmp(option, "-gt", 3) == 0) {
    int threshold = atoi(option + 3); 
                        //BE CAREFUL OF YOUR ALLIGATORS 
    return file_stat->st_size > threshold;
    //DONT FORGET THE EQUALS that was a close one
} else if (strncmp(option, "-eq", 3) == 0) {
    int threshold = atoi(option + 3); 
    return file_stat->st_size == threshold;
}
}


