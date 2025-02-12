%{
#include <stdio.h>
int words = 0;
int chars = 0;
int blanks = 0;
int lines = 0;
%}

%%
\n      { lines++; }
[ \t]   { blanks++; }
[a-zA-Z]+ { words++; chars += yyleng; }
.       { chars++; }
%%

int main() {
    printf("Enter text (Ctrl+D to end input):\n");
    yylex();
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", chars);
    printf("Blanks: %d\n", blanks);
    return 0;
}

int yywrap() {
    return 1;
}
