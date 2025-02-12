/*S→aAcBe
A→Ab|b
B→d*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char *input;
char currentChar;
int position = 0;

void advance();
bool isEnd();
bool parseS();
bool parseA();
bool parseB();

void advance() {
    currentChar = input[position++];
}

bool isEnd() {
    return currentChar == '$';
}

bool parseS() {
    if (currentChar == 'a') {
        advance();
        if (parseA()) {
            if (parseB()) {
                if (currentChar == 'e') {
                    advance();
                    return true;
                }
            }
        }
    }
    return false;
}

bool parseA() {
    if (currentChar == 'b') {
        advance();
        return true;
    } else if (currentChar == 'A') {
        advance();
        if (parseA()) {
            if (currentChar == 'b') {
                advance();
                return true;
            }
        }
    }
    return false;
}

bool parseB() {
    if (currentChar == 'd') {
        advance();
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