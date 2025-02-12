/*S→UVW
U →(S) | aSb | d
V → aV | 𝜖
W → cW | 𝜖*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char *input;
char currentChar;
int position = 0;

bool parseS();
bool parseU();
bool parseV();
bool parseW();

void advance() {
    currentChar = input[position++];
}

bool isEnd() {
    return currentChar == '$';
}

bool parseS() {
    return parseU() && parseV() && parseW();
}

bool parseU() {
    if (isEnd()) return false;

    if (currentChar == '(') {
        advance();
        if (parseS()) {
            if (currentChar == ')') {
                advance();
                return true;
            }
        }
    } else if (currentChar == 'a') {
        advance();
        if (parseS()) {
            if (currentChar == 'b') {
                advance();
                return true;
            }
        }
    } else if (currentChar == 'd') {
        advance();
        return true;
    }
    return false;
}

bool parseV() {
    if (currentChar == 'a') {
        advance();
        return parseV();
    }
    return true;
}

bool parseW() {
    if (currentChar == 'c') {
        advance();
        return parseW();
    }
    return true;
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