/* Write a recursive descent parser for the following simple grammars.

    S→a | > | ( T ) 
    T→T,S |S
*/#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

const char *input;
char currentChar;
int position = 0;

bool parseS();
bool parseT();

void advance() {
    currentChar = input[position++];
}

bool isEnd() {
    return currentChar == '$';
}

bool parseS() {
    if (isEnd()) return false;

    if (currentChar == 'a') {
        advance();
        return true;
    } else if (currentChar == '>') {
        advance();
        return true;
    } else if (currentChar == '(') {
        advance();
        if (parseT()) {
            if (currentChar == ')') {
                advance();
                return true;
            }
        }
    }
    return false;
}

bool parseT() {
    if (parseS()) {
        if (currentChar == ',') {
            advance(); 
            return parseT();
        }
        return true;
    }
    return false;
}

int main() {
    char *exampleInput = (char *)malloc(100 * sizeof(char));
    printf("Enter the input string: ");
    scanf("%s", exampleInput);
    input = exampleInput;

    position = 0;
    advance();

    if (parseS() && isEnd()) {
        printf("Input is valid according to the grammar.\n");
    } else {
        printf("Input is NOT valid according to the grammar.\n");
    }

    return 0;
}