%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char* name;
    char** args;
    int arg_count;
    char* value;
    int is_func;
} Macro;

Macro macros[100];
int macro_cnt = 0;
int collecting_macros = 1;  // Flag for first pass

void add_macro(char* name, char* value, int is_func, char* args) {
    macros[macro_cnt].name = strdup(name);
    macros[macro_cnt].value = strdup(value);
    macros[macro_cnt].is_func = is_func;
    
    if(is_func && args) {
        char* token;
        macros[macro_cnt].args = malloc(5 * sizeof(char*));
        macros[macro_cnt].arg_count = 0;
        
        token = strtok(args, ",");
        while(token) {
            macros[macro_cnt].args[macros[macro_cnt].arg_count++] = strdup(token);
            token = strtok(NULL, ",");
        }
    }
    macro_cnt++;
}

char* expand(char* name, char** actual_args, int arg_count) {
    for(int i=0; i<macro_cnt; i++) {
        if(strcmp(macros[i].name, name) == 0) {
            char* result = strdup(macros[i].value);
            
            if(macros[i].is_func) {
                for(int j=0; j<macros[i].arg_count; j++) {
                    char pattern[20];
                    sprintf(pattern, "%s", macros[i].args[j]);
                    
                    char* pos = strstr(result, pattern);
                    while(pos) {
                        size_t pattern_len = strlen(pattern);
                        size_t actual_len = strlen(actual_args[j]);
                        
                        size_t new_len = strlen(result) - pattern_len + actual_len;
                        char* new_result = malloc(new_len + 1);
                        
                        strncpy(new_result, result, pos - result);
                        strcpy(new_result + (pos - result), actual_args[j]);
                        strcat(new_result, pos + pattern_len);
                        
                        free(result);
                        result = new_result;
                        
                        pos = strstr(result, pattern);
                    }
                }
            }
            return result;
        }
    }
    return NULL;
}

%}

%x DEFINE FUNC_ARGS FUNC_BODY

%%

^[ \t]*#define[ \t]+ {
    if(collecting_macros) {
        char name[50] = {0}, args[100] = {0}, value[200] = {0};
        int is_func = 0;
        char c;
        int idx = 0;
        
        /* Parse macro name */
        while((c = input()) && (isalnum(c) || c == '_'))
            name[idx++] = c;
        name[idx] = '\0';
        unput(c);
        
        /* Check for function-like macro */
        if((c = input()) == '(') {
            is_func = 1;
            idx = 0;
            while((c = input()) != ')') {
                if(c == ' ') continue;
                args[idx++] = c;
            }
            args[idx] = '\0';
        }
        else unput(c);
        
        /* Get macro value */
        idx = 0;
        while((c = input()) != '\n' && c != EOF) {
            if(c == '\\') {  // Handle line continuation
                while((c = input()) == ' ' || c == '\t');
                unput(c);
                continue;
            }
            value[idx++] = c;
        }
        value[idx] = '\0';
        
        add_macro(name, value, is_func, is_func ? args : NULL);
    }
    else {
        /* Skip macro definitions in second pass */
        char c;
        while((c = input()) != '\n' && c != EOF);
    }
}

[a-zA-Z_][a-zA-Z0-9_]* {
    if(!collecting_macros) {
        int found = 0;
        for(int i=0; i<macro_cnt; i++) {
            if(strcmp(yytext, macros[i].name) == 0) {
                if(macros[i].is_func) {
                    int c = input();
                    if(c == '(') {
                        char args[100] = {0};
                        int idx = 0, depth = 1;
                        while((c = input()) != EOF) {
                            if(c == '(') depth++;
                            if(c == ')') if(--depth == 0) break;
                            args[idx++] = c;
                        }
                        args[idx] = '\0';
                        
                        char* tokens[5] = {0};
                        int arg_cnt = 0;
                        char* tok = strtok(args, ",");
                        while(tok) {
                            tokens[arg_cnt++] = strdup(tok);
                            tok = strtok(NULL, ",");
                        }
                        
                        char* expanded = expand(yytext, tokens, arg_cnt);
                        printf("%s", expanded);
                        free(expanded);
                        
                        for(int j=0; j<arg_cnt; j++) free(tokens[j]);
                        found = 1;
                        break;
                    }
                    else unput(c);
                }
                if(!found) {
                    printf("%s", macros[i].value);
                    found = 1;
                    break;
                }
            }
        }
        if(!found) printf("%s", yytext);
    }
}

.|\n {
    if(!collecting_macros) {
        printf("%s", yytext);
    }
}

%%

int yywrap() {
    return 1;
}

int main() {
    /* First pass: collect all macros */
    collecting_macros = 1;
    FILE* f = fopen("input.c", "r");
    if(!f) {
        perror("Error opening input file");
        return 1;
    }
    yyin = f;
    yylex();
    fclose(yyin);
    
    /* Second pass: process the input with macro expansion */
    collecting_macros = 0;
    f = fopen("input.c", "r");
    if(!f) {
        perror("Error opening input file");
        return 1;
    }
    yyin = f;
    yylex();
    fclose(yyin);
    
    /* Print macro table at the end */
    printf("\n\nMacro Table:\n");
    for(int i=0; i<macro_cnt; i++) {
        if(macros[i].is_func) {
            printf("#define %s(", macros[i].name);
            for(int j=0; j<macros[i].arg_count; j++) {
                printf("%s%s", macros[i].args[j], j < macros[i].arg_count-1 ? "," : "");
            }
            printf(") %s\n", macros[i].value);
        }
        else {
            printf("#define %s %s\n", macros[i].name, macros[i].value);
        }
    }
    
    /* Free memory */
    for(int i=0; i<macro_cnt; i++) {
        free(macros[i].name);
        free(macros[i].value);
        if(macros[i].is_func) {
            for(int j=0; j<macros[i].arg_count; j++)
                free(macros[i].args[j]);
            free(macros[i].args);
        }
    }
    
    return 0;
}