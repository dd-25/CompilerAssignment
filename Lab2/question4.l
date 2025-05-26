%{
#include <stdio.h>
int for_depth = 0;
int for_count = 0; 
%}

%%

[ \t\n]+         ;  
for              { for_depth++; for_count++; if (for_depth > 1) printf("Nested 'for' loop detected!\n"); }
\{               ; 
\}               { if (for_depth > 0) for_depth--; } 
.                ;
%%

int main() {
    yylex();
    printf("Total 'for' loops: %d\n", for_count);
    return 0;
}

int yywrap() {
    return 1;
}
