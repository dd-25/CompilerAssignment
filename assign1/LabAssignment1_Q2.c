#include<stdio.h>
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "include", "stdlib.h", "stdio.h", "main", "printf"};
char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
char digits[] = "0123456789";

// char operator[] = "+-=/*<>"

int row = 0;
int col = 0;

struct symbolTable{
    char *identifierType;
    int colNo;
    int rowNo;
};

struct symbolTable table[100];
int symbolTableIdx = 0;

char *operators[] = {"+", "-", "*", "/", "%", "=", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>"};


int isMatch(char *text, int length) {
    // Check for keywords
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strncmp(text, keywords[i], length) == 0 && strlen(keywords[i]) == length) {
            return 1; // Match found: Keyword
        }
    }

    // Check for operators
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strncmp(text, operators[i], length) == 0 && strlen(operators[i]) == length) {
            return 4; // Match found: Operator
        }
    }

    // Check if it's a valid identifier
    if (strchr(alphabets, text[0])) {
        for (int i = 1; i < length; i++) {
            if (!isalnum(text[i]) && text[i] != '_') {
                return 0; // Invalid identifier
            }
        }
        return 2; // Valid identifier
    }

    // Check if it's a number
    for (int i = 0; i < length; i++) {
        if (!isdigit(text[i])) {
            return 0; // Invalid number
        }
    }
    return 3; 
}



void parse(char *text, int length) {
    int i = 0;
    int start = 0; 
    char currentToken[256]; 

    while (i < length) {
        // Skip leading whitespace and update row/column positions
        while (i < length && (text[i] == ' ' || text[i] == '\t' || text[i] == '\n')) {
            if (text[i] == '\n') {
                row++;
                col = 0; // Reset column at a new line
            } else {
                col++;
            }
            i++;
        }

        // Check if the current token starts a string
        if (i < length && text[i] == '"') {
            start = i++;
            while (i < length && text[i] != '"') {
                i++;
                col++;
            }
            if (i < length && text[i] == '"') {
                i++;
                col++;
            }
            int tokenLength = i - start;
            strncpy(currentToken, &text[start], tokenLength);
            currentToken[tokenLength] = '\0'; // Null-terminate the token
            table[symbolTableIdx].colNo = col - tokenLength;
            table[symbolTableIdx].rowNo = row;
            table[symbolTableIdx].identifierType = currentToken;
            printf("String: %s at row: %d, col: %d\n", currentToken, row, col - tokenLength);
            continue;
        }

        // Reset the token buffer
        start = i;
        while (i < length && text[i] != ' ' && text[i] != '\t' && text[i] != '\n') {
            i++;
            col++;
        }

        // Extract and process the token
        if (start < i) {
            int tokenLength = i - start;
            strncpy(currentToken, &text[start], tokenLength);
            currentToken[tokenLength] = '\0'; // Null-terminate the token

            int matchType = isMatch(currentToken, tokenLength);
            if (matchType == 1) {
                printf("Keyword: %s at row: %d, col: %d\n", currentToken, row, col - tokenLength);
            } else if (matchType == 2) {
                printf("Identifier: Variable, Name: %s at row: %d, col: %d\n", currentToken, row, col - tokenLength);
            } else if (matchType == 3) {
                printf("Number: Literal, Value: %s at row: %d, col: %d\n", currentToken, row, col - tokenLength);
            } else {
                printf("Unknown Token: %s at row: %d, col: %d\n", currentToken, row, col - tokenLength);
            }
        }
    }
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
    buffer[fileSize] = '\0';
    fclose(file);

    char code[] = "int a = 5";
    int length = sizeof(code) - 1;

    int row = 0;
    int col = 0;

    parse(buffer, fileSize);

    for (int i = 0; i < symbolTableIdx; i++) {
        printf("Type : %s | rowNo : %d | colNo : %d\n",
               table[i].identifierType, table[i].rowNo, table[i].colNo);
    }

    free(buffer);
    return 0;
}
