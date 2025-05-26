%{
#include <stdio.h>

int count = 0;
%}

%option noyywrap

%%

[!@#$%^&*()_+=\-[\]{};':"\\|,.<>/?][a-zA-Z0-9]* {
    count++; 
    printf("Matched: %s\n", yytext); 
}


[\n] {
    printf("The number of words starting with special symbols are : %d\n",count);
}

%%

int main() {
    yylex(); 

    printf("Number of strings with at least one special symbol: %d\n", count);

    return 0;
}
