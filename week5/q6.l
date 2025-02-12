%{
#include <stdio.h>
%}

%%
\"[^\"]*\"|\'[^\']*\'|\/\/[^\n]*|\/\*[^*]*\*+([^/*][^*]*\*+)*\/ { ECHO; }
[a-z] { putchar(yytext[0] - 'a' + 'A'); }
[A-Z] { putchar(yytext[0] - 'A' + 'a'); }
.     { putchar(yytext[0]); }
%%

int main() {
    printf("Enter C code (Ctrl+D to end input):\n");
    yylex();
    return 0;
}
int yywrap() {
    return 1;
}
