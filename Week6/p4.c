/*S →(L) | a
L → L,S | S*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char *input;
char currentChar;
int position = 0;

void advance();
bool isEnd();
bool parseS();
bool parseL();

void advance() {
    currentChar = input[position++];
}

bool isEnd() {
    return currentChar == '$';
}

bool parseS() {
    if (currentChar == '(') {
        advance();
        if (parseL()) {
            if (currentChar == ')') {
                advance();
                return true;
            }
        }
    } else if (currentChar == 'a') {
        advance();
        return true;
    }
    return false;
}

bool parseL() {
    if (parseS()) {
        if (currentChar == ',') {
            advance();
            return parseL();
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