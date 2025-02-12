%{
#include <stdio.h>
int pos_int = 0;
int neg_int = 0;
int pos_float = 0;
int neg_float = 0;
%}

%%
[0-9]+        { pos_int++; }
-[0-9]+       { neg_int++; }
[0-9]+\.[0-9]+ { pos_float++; }
-[0-9]+\.[0-9]+ { neg_float++; }
.             { /* Ignore other characters */ }
%%

int main() {
    printf("Enter text (Ctrl+D to end input):\n");
    yylex();
    printf("Positive integers: %d\n", pos_int);
    printf("Negative integers: %d\n", neg_int);
    printf("Positive floating-point numbers: %d\n", pos_float);
    printf("Negative floating-point numbers: %d\n", neg_float);
    return 0;
}
int yywrap() {
    return 1;
}

