#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Function to check if a number already exists in the constants array
bool already_exists(const char temp[], char *constants[], int constants_count) {
    for (int i = 0; i < constants_count; i++) {
        if (strcmp(temp, constants[i]) == 0) {
            return true;
        }
    }
    return false;
}

int main() {
    char *constants[1000]; // Array to store numeric constants
    int constants_count = 0;

    FILE *file;
    char filename[100] = "sample.c"; // Change this to the name of your input file
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return 1;
    }

    char ch;
    char temp[256]; // Temporary buffer for reading constants or strings
    int temp_index = 0;

    while ((ch = fgetc(file)) != EOF) {
        // Skip preprocessor directives
        if (ch == '#') {
            while ((ch = fgetc(file)) != EOF && ch != '\n');
            continue;
        }

        // Skip single-line comments
        if (ch == '/') {
            char next_char = fgetc(file);
            if (next_char == '/') {
                while ((ch = fgetc(file)) != EOF && ch != '\n');
                continue;
            } else if (next_char == '*') {
                // Skip multi-line comments
                while ((ch = fgetc(file)) != EOF) {
                    if (ch == '*' && (ch = fgetc(file)) == '/') {
                        break;
                    }
                }
                continue;
            } else {
                ungetc(next_char, file);
            }
        }
        // Detect string literals
        if (ch == '"') {
            temp_index = 0;
            while ((ch = fgetc(file)) != EOF && ch != '"') {
                if (temp_index < sizeof(temp) - 1) {
                    temp[temp_index++] = ch;
                }
            }
            temp[temp_index] = '\0'; // Null-terminate the string

            // Add the string literal to the array
            if(!already_exists(temp, constants, constants_count))
            {
                constants[constants_count] = (char *)malloc(strlen(temp) + 1);
                if (constants[constants_count] == NULL) {
                    printf("Error: Memory allocation failed\n");
                    fclose(file);
                    return 1;
                }
                strcpy(constants[constants_count], temp);
                constants_count++;
            }
            continue;
        }

        // Detect numeric constants
        if (isdigit(ch)) {
            temp_index = 0;
            bool point_seen = false; // Flag to track decimal point

            while (isdigit(ch) || (ch == '.' && !point_seen)) {
                if (ch == '.') {
                    point_seen = true; // Mark the decimal point as seen
                }

                if (temp_index < sizeof(temp) - 1) {
                    temp[temp_index++] = ch;
                }
                ch = fgetc(file);
            }
            temp[temp_index] = '\0'; // Null-terminate the constant

            // Add the constant to the array if it doesn't already exist
            if (!already_exists(temp, constants, constants_count)) {
                constants[constants_count] = (char *)malloc(strlen(temp) + 1);
                if (constants[constants_count] == NULL) {
                    printf("Error: Memory allocation failed\n");
                    fclose(file);
                    return 1;
                }
                strcpy(constants[constants_count], temp);
                constants_count++;
            }

            // If `ch` is not EOF, push it back
            if (ch != EOF) {
                ungetc(ch, file);
            }
            continue;
        }

        // Skip character literals
        if (ch == '\'') {
            temp_index = 0;
            while ((ch = fgetc(file)) != EOF && ch != '\'') {
                if (temp_index < sizeof(temp) - 1) {
                    temp[temp_index++] = ch;
                }
            }
            temp[temp_index] = '\0'; // Null-terminate the string

            // Add the string literal to the array
            if (!already_exists(temp, constants, constants_count)) {
                constants[constants_count] = (char *)malloc(strlen(temp) + 1);
                if (constants[constants_count] == NULL) {
                    printf("Error: Memory allocation failed\n");
                    fclose(file);
                    return 1;
                }
                strcpy(constants[constants_count], temp);
                constants_count++;
            }
            continue;
        }

    }

    fclose(file);

    // Print the collected constants
    printf("Total Constants: %d\n", constants_count);
    for (int i = 0; i < constants_count; i++) {
        printf("%s\n", constants[i]);
        free(constants[i]); // Free allocated memory
    }

    // Print the collected string literals
    // printf("\nTotal String Literals: %d\n", constants_count);
    // for (int i = 0; i < constants_count; i++) {
    //     printf("\"%s\"\n", constants[i]);
    //     free(constants[i]); // Free allocated memory
    // }

    return 0;
}