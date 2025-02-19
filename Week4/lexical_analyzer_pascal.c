#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getNT.h"

char keywords[9][10] = {"begin", "end", "if", "then", "else", "for", "while", "function", "procedure"};

int handleComments() {
    char c = fgetc(fp);
    int flag = 0;
    if (c == '{') {
        flag = 1;
        do {
            c = fgetc(fp);
            col++;
            if (c == '\n') {
                printf("\n");
                row++;
                col = 1;
            }
        } while (c != '}' && c != EOF);
    } else {
        ungetc(c, fp);
    }
    return flag;
}

// Add any additional handling for Pascal specific tokens and syntax if needed