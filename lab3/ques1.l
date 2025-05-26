%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Variables to store parts of for loop */
char init[1000];
char cond[1000];
char incr[1000];

/* Variable to track if we're inside a do-while loop */
int in_do_while = 0;
char do_while_cond[1000];

/* Function to trim leading and trailing whitespace */
void trim(char *str) {
    char *start = str;
    char *end;
    
    /* Trim leading whitespace */
    while (isspace(*start)) start++;
    
    if (*start == '\0') {
        /* String contains only whitespace */
        *str = '\0';
        return;
    }
    
    /* Trim trailing whitespace */
    end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    
    /* Write new null terminator */
    *(end + 1) = '\0';
    
    /* Move the trimmed string to the beginning */
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}
%}

%option noyywrap

%%
"for"[ \t]*"("[^;]*";"[^;]*";"[^)]*")" {
    /* Extract the three parts of the for loop */
    char *content = strdup(yytext + 3); /* Skip "for" */
    trim(content);
    /* Find the first semicolon */
    char *firstSemi = strchr(content, ';');
    if (firstSemi) {
        *firstSemi = '\0';
        strcpy(init, content + 1); /* Skip the opening parenthesis */
        trim(init);
        
        /* Find the second semicolon */
        char *secondSemi = strchr(firstSemi + 1, ';');
        if (secondSemi) {
            *secondSemi = '\0';
            strcpy(cond, firstSemi + 1);
            trim(cond);
            
            /* Find the closing parenthesis */
            char *closeParen = strchr(secondSemi + 1, ')');
            if (closeParen) {
                *closeParen = '\0';
                strcpy(incr, secondSemi + 1);
                trim(incr);
                
                /* Output the while loop format */
                fprintf(yyout, "%s;\nwhile(%s) ", init, cond);
                /* The closing brace and increment will be handled in the main function */
            }
        }
    }
    
    free(content);
}

"do"[ \t]*"{" {
    /* Mark that we're entering a do-while loop */
    in_do_while++;
    
    /* Output a plain while(1) loop */
    fprintf(yyout, "while(1) {");
}

"}"[ \t]*"while"[ \t]*"("[^)]*");" {
    if (in_do_while) {
        /* Extract the condition from the do-while loop */
        char *start = strchr(yytext, '(');
        char *end = strrchr(yytext, ')');
        
        if (start && end && end > start) {
            /* Extract and store the condition */
            int len = end - start - 1;
            strncpy(do_while_cond, start + 1, len);
            do_while_cond[len] = '\0';
            trim(do_while_cond);
            
            /* Output if statement with break */
            fprintf(yyout, "    if(!(%s)) break;\n}", do_while_cond);
            
            /* Reset the do-while state */
            in_do_while--;
        } else {
            /* If we can't parse it properly, just output as is */
            fprintf(yyout, "%s", yytext);
        }
    } else {
        /* Not a do-while we recognized, output as is */
        fprintf(yyout, "%s", yytext);
    }
}

"}"\n* {
    /* Check if we need to add increment before closing brace */
    if (strlen(incr) > 0) {
        fprintf(yyout, "    %s;\n}", incr);
        /* Reset the increment for the next for loop */
        incr[0] = '\0';
    } else {
        fprintf(yyout, "%s", yytext);
    }
}

. { 
    /* Output all other characters unchanged */
    fprintf(yyout, "%s", yytext); 
}

\n { 
    /* Preserve newlines */
    fprintf(yyout, "\n"); 
}
%%

int main(int argc, char *argv[]) {
    /* Open input file */
    yyin = fopen("input.c", "r");
    if (!yyin) {
        fprintf(stderr, "Could not open input.c\n");
        return 1;
    }
    
    /* Open output file */
    yyout = fopen("output.c", "w");
    if (!yyout) {
        fprintf(stderr, "Could not open output.c\n");
        fclose(yyin);
        return 1;
    }
    
    /* Initialize variables */
    init[0] = '\0';
    cond[0] = '\0';
    incr[0] = '\0';
    do_while_cond[0] = '\0';
    in_do_while = 0;
    
    /* Process input */
    yylex();
    
    /* Close files */
    fclose(yyin);
    fclose(yyout);
    
    return 0;
}
