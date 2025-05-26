#include <stdio.h>
#include <stdbool.h>
//to check ki digit valid hai yaa nahin
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isUnsignedNumber(const char *input) {
    int currentState = 0;// matlab abhi start of string hai 

//to parse the input
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
//state transition ke liye abhi
        switch (currentState) {
            case 0:
                if (isDigit(c)) {
                    currentState = 1; //matlab abhi tak valid hai 
                } else {
                    currentState = 2;//digit nahin hai matlab invalid (error/dead state)
                }
                break;

            case 1:
                if (isDigit(c)) {
                    currentState = 1;
                } else {
                    currentState = 2;
                }
                break;

            case 2:
                return false;
        }
    }

    return currentState == 1;//yadi last tak digit hi rha hai then valid hai
}

int main() {
    char input[100];

    printf("Enter the string to check: ");
    scanf("%s", input);

    if (isUnsignedNumber(input)) {
        printf("The string \"%s\" is a valid unsigned number.\n", input);
    } else {
        printf("The string \"%s\" is not a valid unsigned number.\n", input);
    }

    return 0;
}
