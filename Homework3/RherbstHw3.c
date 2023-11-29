#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Maximum length of the file name
// Make it large to accommodate file names
#define MAX_FILENAME_LENGTH 1024

// Function to perform Caesar cipher encryption or decryption
// For each character in the text file, determine if it's lowercase or uppercase and an alphabet character
void caesar_cipher(char *text, int offset) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            // Determine the base character for the case (lowercase or uppercase)
            char base = islower(text[i]) ? 'a' : 'A';
            // Apply the Caesar cipher with the specified offset
            text[i] = (text[i] - base + offset) % 26 + base;
        }
    }
}

// Function to encrypt a file based on the specified cipher type
//this is why I will read the homework as a whole instead of doing a checklist
//Yay "easy part" dont have to deal with the structures
void encrypt_file(const char *filename, int offset, const char *output_filename, const char *cipher_type) {
    //make sure its "r" for read
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }
    //make sure its "w" for write
    //if you diont do this write... it dont work
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(file);
        return;
    }
    //really dont know how big or small to go so im justy going big
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strcmp(cipher_type, "Caesar") == 0) {
            // Encrypt the text using Caesar cipher
            caesar_cipher(buffer, offset);
        } else {
            // Encrypt the text using random cipher
            random_cipher(buffer);
        }
        // Write the encrypted text to the output file
        fputs(buffer, output_file);
    }

    // Close the input and output files
    fclose(file);
    fclose(output_file);
}

// Function to decrypt a file based on the specified cipher type
//same thing just opposite
void decrypt_file(const char *filename, int offset, const char *output_filename, const char *cipher_type) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(file);
        return;
    }

    char buffer[1024];
    if (strcmp(cipher_type, "Caesar") == 0) {
        // If decrypting with Caesar cipher, read the offset from the input file
        int offset2;
        fscanf(file, "%d", &offset2);

        while (fgets(buffer, sizeof(buffer), file)) {
            // Decrypt the text using Caesar cipher with the provided offset
            caesar_cipher(buffer, offset2);
            // Write the decrypted text to the output file
            fputs(buffer, output_file);
        }
    } else {
        // Decrypt the text using the random cipher
        while (fgets(buffer, sizeof(buffer), file)) {
            //decrypt using random
            random_cipher(buffer);
            // Write the decrypted text to the output file
            fputs(buffer, output_file);
        }
    }

    // Close the input and output files
    fclose(file);
    fclose(output_file);
}



// Function to perform a random cipher encryption
void random_cipher(char *text) {
    // Initialize the alphabet for random cipher
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    //char capalphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // Create an array to store the shuffled alphabet
    //the 23 means we we only want a shuffled alphabet in there
    char shuffled_alphabet[26];
    strcpy(shuffled_alphabet, alphabet);

  
    //do the random generation into the aplhabet
    //this reminds me of magnets...How do they work??????????How does this work???

    srand((unsigned int)time(NULL))

    // Shuffle the alphabet to create a random mapping
    //Mostly the same for this 
    for (int i = 0; i < 26; i++) {
        int j = rand() % 26;
        char temp = shuffled_alphabet[i];
        shuffled_alphabet[i] = shuffled_alphabet[j];
        shuffled_alphabet[j] = temp;
    }

    // Apply the random cipher to the text
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            // Determine the base character for the case (lowercase or uppercase)
            char base = islower(text[i]) ? 'a' : 'A';
            // Use the shuffled alphabet to encrypt the character
            text[i] = shuffled_alphabet[text[i] - base];
        }
    }
}

int main() {
    char user_input[18];
    do {
        // User input for continuing the program
        printf("Do you wish to continue (yes/no)? ");
        scanf("%s", user_input);

        if (strcmp(user_input, "no") == 0 || strcmp(user_input, "n") == 0) {
            break;
        }

        char action[16];
        // User input for encryption or decryption
        printf("Do you want to encrypt or decrypt a file? ");
        scanf("%s", action);

        if (strcmp(action, "encrypt") == 0) {
            char filename[MAX_FILENAME_LENGTH];
            // User input for input file name
            printf("Enter the file name: ");
            scanf("%s", filename);

            int offset;
            // User input for the offset value
            printf("Enter the offset: ");
            scanf("%d", &offset);

            char output_filename[MAX_FILENAME_LENGTH];
            // User input for output file name
            printf("Enter the output file name: ");
            scanf("%s", output_filename);

            char cipher_type[16];
            // User input for cipher type
            printf("Enter 'Caesar' or 'Random' cipher: ");
            scanf("%s", cipher_type);

            // Encrypt the file using the specified options
            encrypt_file(filename, offset, output_filename, cipher_type);
            printf("File '%s' encrypted using %s cipher and saved as '%s'.\n", filename, cipher_type, output_filename);
        } else if (strcmp(action, "decrypt") == 0) {
            char filename[MAX_FILENAME_LENGTH];
            // User input for input file name
            printf("Enter the input file: ");
            scanf("%s", filename);

            int offset;
            // User input for the offset value
            printf("Enter the offset: ");
            scanf("%d", &offset);

            char output_filename[MAX_FILENAME_LENGTH];
            // User input for output file name
            printf("Enter the output : ");
            scanf("%s", output_filename);

            char cipher_type[16];
            // User input for cipher type
            printf("Enter 'Caesar' or 'Random': ");
            scanf("%s", cipher_type);

            // Decrypt the file using the specified options
            decrypt_file(filename, offset, output_filename, cipher_type);
            printf("'%s' decrypted using %s cipher and saved as '%s'.\n", filename, cipher_type, output_filename);
        } else {
            printf("Enter 'encrypt' or 'decrypt'.\n");
        }
    } while (1);

    return 0;
}
