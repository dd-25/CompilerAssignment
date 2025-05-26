%{
#include <stdio.h>

char str[1000]; 
int idx = 0;   
%}

%option noyywrap

%%


"//".* { /* Ignore single-line comments */ }


"/*"([^*]|"*"+[^*/])*"*"+"/" { /* Ignore multi-line comments */ }


[ \t]+ { /* Ignore whitespaces */ }

[\n] {
    str[idx++] = '\n'; // Preserve newlines for readability
    printf("%s\n", str);
}


. {
    str[idx++] = yytext[0]; // Add the character to the buffer
}

%%

int main() {
    yylex(); // Call the lexer

    // Print the cleaned program
    printf("Cleaned Program:\n%s\n", str);

    return 0;
}
