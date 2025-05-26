%{
#include <stdio.h>

int count = 0;
%}

%option noyywrap

%%

^s.*d$ {
    count++; 
    printf("Matched: %s\n", yytext); 
}

[\n] {
    printf("The count of matching words is : %d\n",count);
}

%%

int main() {
    yylex(); 

    printf("Number of strings starting with 's' and ending with 'd': %d\n", count);

    return 0;
}
