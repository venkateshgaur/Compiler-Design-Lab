#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getNT.h"

char keywords[9][10] = {"function", "end", "if", "else", "elseif", "for", "while", "return", "classdef"};

int handleComments() {
    char c = fgetc(fp);
    int flag = 0;
    if (c == '%') {
        flag = 1;
        do {
            c = fgetc(fp);
            col++;
        } while (c != '\n' && c != EOF);
        printf("\n");
        row++;
        col = 1;
    } else {
        ungetc(c, fp);
    }
    return flag;
}

// Add any additional handling for MATLAB specific tokens and syntax if needed