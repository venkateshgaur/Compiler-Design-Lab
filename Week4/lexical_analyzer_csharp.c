#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getNT.h"

char keywords[9][10] = {"int", "float", "char", "long", "for", "if", "else", "while", "class"};

int handleComments() {
    char c = fgetc(fp);
    int flag = 0;
    if (c == '/') {
        c = fgetc(fp);
        col++;
        if (c == '/') {
            flag = 1;
            do {
                c = fgetc(fp);
                col++;
            } while (c != '\n' && c != EOF);
            printf("\n");
            row++;
            col = 1;
        } else if (c == '*') {
            char prev = '\0';
            do {
                c = fgetc(fp);
                col++;
                if (prev == '*' && c == '/') {
                    flag = 1;
                    break;
                } else if (c == '\n') {
                    printf("\n");
                    row++;
                    col = 1;
                }
                prev = c;
            } while (c != EOF);
        } else {
            ungetc(c, fp);
        }
    } else {
        ungetc(c, fp);
    }
    return flag;
}

// Add any additional handling for C# specific tokens and syntax if needed