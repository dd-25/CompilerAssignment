#include <stdio.h>
#include <stdlib.h>

int countNewLine(char *text, int length) {
    int lines = 1;
    for (int i = 0; i < length; i++) {
        if (text[i] == '\n') {
            lines++;
        }
    }
    return lines;
}

int main() {
    FILE *file = fopen("sample.c", "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0'; // Null-terminate the string

    int lineCount = countNewLine(buffer, fileSize);

    printf("The file contains %d lines.\n", lineCount);

    // Clean up
    free(buffer);
    fclose(file);

    return 0;
}
