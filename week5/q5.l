%{
#include <stdio.h>
%}

%%
[0-9]+ { printf("0x%X\n", atoi(yytext)); }
.      { putchar(yytext[0]); }
%%

int main() {
    printf("Enter a decimal number (Ctrl+D to end input):\n");
    yylex();
    return 0;
}
int yywrap() {
    return 1;
}
