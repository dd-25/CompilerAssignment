%{
/* C declarations used in actions */
#include <stdio.h>

/* List of C keywords */
int is_keyword(char *s) {
    char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", 
        "do", "double", "else", "enum", "extern", "float", "for", "goto", 
        "if", "int", "long", "register", "return", "short", "signed", 
        "sizeof", "static", "struct", "switch", "typedef", "union", 
        "unsigned", "void", "volatile", "while"
    };
    
    int n = sizeof(keywords)/sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(s, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
%}

/* Lex definitions */
%option noyywrap

%%
[ \t\n]+                   { ECHO; }  /* Echo whitespace as is */
"#".$                     { ECHO; }  / Echo preprocessor directives as is */

"/"([^]|""[^/])"/"    { ECHO; }  / Handle C-style comments */
"//".$                    { ECHO; }  / Handle C++ style comments */

[a-zA-Z_][a-zA-Z0-9_]*     {
                            /* Check if it's a keyword or identifier */
                            if (is_keyword(yytext)) {
                                printf("%s<KW>", yytext);
                            } else {
                                /* Check if it's not a standard library function like printf */
                                if (strcmp(yytext, "printf") == 0 || 
                                    strcmp(yytext, "scanf") == 0 ||
                                    strcmp(yytext, "main") == 0) {
                                    ECHO;  /* Print standard functions as is */
                                } else {
                                    printf("%s<ID>", yytext);
                                }
                            }
                           }

\"([^\"]|\\.)\"           { ECHO; }  / Echo string literals as is */
'.'|'\\.'                  { ECHO; }  /* Echo character literals as is */
[0-9]+                     { ECHO; }  /* Echo numbers as is */
.                          { ECHO; }  /* Echo other characters as is */
%%

int main(int argc, char **argv) {
    /* If filename is specified, open it for reading */
    if (argc > 1) {
        FILE *file;
        file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            exit(1);
        }
        yyin = file;
    }
    
    yylex();
    return 0;
}
