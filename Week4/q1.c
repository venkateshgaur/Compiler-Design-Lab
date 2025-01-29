#include<stdio.h>
#include<string.h>S
#include"buildfun.h"
#include<ctype.h> 
int isArithmeticOp(char *c)
{
    return (
        strcmp(c, "+") == 0 ||
        strcmp(c, "-") == 0 ||
        strcmp(c, "*") == 0 ||
        strcmp(c, "/") == 0 ||
        strcmp(c, "++") == 0 ||
        strcmp(c, "--") == 0 ||
        strcmp(c, "%") == 0);
}

int isLogicalOp(char *c)
{
    return (
        strcmp(c, "||") == 0 ||
        strcmp(c, "&&") == 0 ||
        strcmp(c, "!=") == 0);
}

int isRelationOp(char *c)
{
    return (
        strcmp(c, "<") == 0 ||
        strcmp(c, ">") == 0 ||
        strcmp(c, "<=") == 0 ||
        strcmp(c, ">=") == 0 ||
        strcmp(c, "=") == 0 ||
        strcmp(c, "==") == 0);
}

int isSpecialSymbol(char *c)
{
    return (
        strcmp(c, ";") == 0 ||
        strcmp(c, ",") == 0 ||
        strcmp(c, "(") == 0 ||
        strcmp(c, ")") == 0 ||
        strcmp(c, "{") == 0 ||
        strcmp(c, "}") == 0 ||
        strcmp(c, "[") == 0 ||
        strcmp(c, "]") == 0 ||
        strcmp(c, ".") == 0 ||
        strcmp(c, "&") == 0 ||
        strcmp(c, "|") == 0 ||
        strcmp(c, "^") == 0 ||
        strcmp(c, "~") == 0 ||
        strcmp(c, "?") == 0 ||
        strcmp(c, ":") == 0);
}

int isKeyword(char *c)
{
    char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
        "inline", "restrict", "bool", "complex", "imaginary"};

    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(c, keywords[i]) == 0)
        {
            return 1; // It's a keyword
        }
    }
    return 0; // Not a keyword
}

int isNumericalConstant(char *c)
{
    int hasDecimalPoint = 0, i = 0;
    // Check for optional sign
    if (c[i] == '+' || c[i] == '-')
    {
        i++;
    }
    // Check digits
    while (c[i] != '\0')
    {
        if (c[i] == '.')
        {
            if (hasDecimalPoint)
            {
                return 0; // More than one decimal point
            }
            hasDecimalPoint = 1; // Found a decimal point
        }
        else if (!isdigit(c[i]))
        {
            return 0;
        }
        i++;
    }
    return 1;
}