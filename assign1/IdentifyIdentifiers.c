#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Function to check if a given word is a keyword
bool is_keyword(const char *word) {
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "printf", "main"
    };
    int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return true; // Word is a keyword
        }
    }
    return false; // Word is not a keyword
}

// Function to check if an identifier already exists in the array
bool already_exists(char temp[], char *identifiers[], int identifiers_count) {
    for (int i = 0; i < identifiers_count; i++) {
        if (strcmp(temp, identifiers[i]) == 0) {
            return true;
        }
    }
    return false;
}

int main() {
    char *identifiers[1000]; // Array to store identifiers
    int identifiers_count = 0;

    FILE *file;
    char filename[100] = "sample.c"; // Change this to the name of your input file
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return 1;
    }

    char ch;
    char temp[256]; // Temporary buffer for reading words
    int temp_index = 0;

    while ((ch = fgetc(file)) != EOF) {
        // Skip string literals
        if (ch == '"') {
            while ((ch = fgetc(file)) != EOF && ch != '"');
            continue;
        }

        // Skip character literals
        if (ch == '\'') {
            while ((ch = fgetc(file)) != EOF && ch != '\'');
            continue;
        }

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

        // Check for identifiers
        if (isalpha(ch) || ch == '_') {
            temp_index = 0;
            temp[temp_index++] = ch;

            // Read the rest of the identifier
            while ((ch = fgetc(file)) != EOF && (isalnum(ch) || ch == '_')) {
                if (temp_index < sizeof(temp) - 1) { // Prevent buffer overflow
                    temp[temp_index++] = ch;
                }
            }
            temp[temp_index] = '\0'; // Null-terminate the string

            // Check if it's a keyword or already exists
            if (!is_keyword(temp) && !already_exists(temp, identifiers, identifiers_count)) {
                // Add to the list of identifiers
                identifiers[identifiers_count] = (char *)malloc(strlen(temp) + 1);
                if (identifiers[identifiers_count] == NULL) {
                    printf("Error: Memory allocation failed\n");
                    fclose(file);
                    return 1;
                }
                strcpy(identifiers[identifiers_count], temp);
                identifiers_count++;
            }

            // Only push back if `ch` is not EOF (avoid undefined behavior)
            if (ch != EOF) {
                ungetc(ch, file);
            }
        }
    }

    fclose(file);

    // Print the collected identifiers
    printf("Total Identifiers: %d\n", identifiers_count);
    for (int i = 0; i < identifiers_count; i++) {
        printf("%s\n", identifiers[i]);
        free(identifiers[i]); // Free allocated memory
    }

    return 0;
}