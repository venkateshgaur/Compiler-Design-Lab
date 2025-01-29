#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_TOKENS 1000

typedef enum
{
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_COMMENT,
    TOKEN_PREPROCESSOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct
{
    int row;
    int col;
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

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

Token getNextToken(FILE *source, int *row, int *col)
{
    Token token;
    token.row = *row;
    token.col = *col;
    token.type = TOKEN_UNKNOWN;
    token.value[0] = '\0';

    int c;
    while ((c = fgetc(source)) != EOF)
    {
        (*col)++;

        // Handle new lines
        if (c == '\n')
        {
            (*row)++;
            *col = 0;
            continue;
        }

        // Skip whitespace
        if (isspace(c))
        {
            continue;
        }

        // Handle comments
        if (c == '/')
        {
            c = fgetc(source);
            if (c == '/')
            { // Single-line comment
                while ((c = fgetc(source)) != EOF && c != '\n')
                {
                    (*col)++;
                }
                (*row)++;
                *col = 0;
                continue;
            }
            else if (c == '*')
            { // Multi-line comment
                while (1)
                {
                    c = fgetc(source);
                    if (c == EOF)
                        break;
                    if (c == '*')
                    {
                        c = fgetc(source);
                        if (c == '/')
                        {
                            break;
                        }
                    }
                    if (c == '\n')
                    {
                        (*row)++;
                        *col = 0;
                    }
                }
                continue;
            }
            else
            {
                ungetc(c, source); // Not a comment, put back the character
                c = '/';
            }
        }

        // Handle preprocessor directives
        if (c == '#')
        {
            token.type = TOKEN_PREPROCESSOR;
            token.value[0] = c;
            int i = 1;
            while ((c = fgetc(source)) != EOF && c != '\n')
            {
                token.value[i++] = c;
                (*col)++;
            }
            token.value[i] = '\0';
            return token;
        }

        // Handle string literals
        if (c == '"')
        {
            token.type = TOKEN_STRING;
            int i = 0;
            token.value[i++] = c;
            while ((c = fgetc(source)) != EOF)
            {
                token.value[i++] = c;
                (*col)++;
                if (c == '"')
                {
                    break;
                }
            }
            token.value[i] = '\0';
            return token;
        }

        // Handle identifiers and keywords
        if (isalpha(c) || c == '_')
        {
            token.type = TOKEN_ID;
            int i = 0;
            token.value[i++] = c;
            while (isalnum((c = fgetc(source))) || c == '_')
            {
                token.value[i++] = c;
                (*col)++;
            }
            ungetc(c, source);
            token.value[i] = '\0';
            if (isKeyword(token.value))
            {
                token.type = TOKEN_KEYWORD;
            }
            return token;
        }

        // Handle numbers
        if (isdigit(c))
        {
            token.type = TOKEN_NUMBER;
            int i = 0;
            token.value[i++] = c;
            while (isdigit((c = fgetc(source)))){
                token.value[i++] = c;
                (*col)++;
            }
            ungetc(c, source);
            token.value[i] = '\0';
            return token;
        }

        // Handle operators
        token.type = TOKEN_OPERATOR;
        token.value[0] = c;
        token.value[1] = '\0';
        
        char output[512];
        // Check for 2-character operators
        if (c == '+' || c == '-' || c == '!' || c == '<' || c == '>')
        {
            char next = fgetc(source);
            if (next == c || next == '=' || next == '-')
            { // Handle ++, --, ==, !=, <=, >=
                token.value[1] = next;
                token.value[2] = '\0';
            }
            else
            {
                ungetc(next, source); // Put back the character if it's not part of an operator
            }
        }

        // Now, print the classification
        if (isArithmeticOp(token.value))
        {
            snprintf(output, sizeof(output), "<%s, Arithmetic operator, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }
        else if (isRelationOp(token.value))
        {
            snprintf(output, sizeof(output), "<%s, Relational Operator, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }
        else if (isLogicalOp(token.value))
        {
            snprintf(output, sizeof(output), "<%s, Logical Operator, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }
        else if (isNumericalConstant(token.value))
        {
            snprintf(output, sizeof(output), "<%s, num, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }
        else if (isSpecialSymbol(token.value))
        {
            snprintf(output, sizeof(output), "<%s, Special Symbol, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }
        else if (isKeyword(token.value))
        {
            snprintf(output, sizeof(output), "<%s, Keyword, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }
        else
        {
            snprintf(output, sizeof(output), "<%s, Identifier, %d, %d>\n", token.value, token.row, token.col);
            printf("%s", output);
        }

        return token;
    }

    token.type = TOKEN_EOF;
    return token;
}