%{
#include <stdio.h>
int scanf_count = 0;
int printf_count = 0;
%}

%%
scanf      { printf("READ"); scanf_count++; }
printf     { printf("WRITE"); printf_count++; }
.          { printf("%c",yytext[0]); }
%%

int main() {
    printf("Enter C code (Ctrl+D to end input):\n");
    yylex();
    printf("\nNumber of READ statements: %d\n", scanf_count);
    printf("Number of WRITE statements: %d\n", printf_count);
    return 0;
}
int yywrap() {
    return 1;
}
