#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEYWORDS 20
#define MAX_LINE_LENGTH 100

char *keywords[MAX_KEYWORDS] = {
    "int", "char", "float", "continue", "break", "do", "while",
    "for", "if", "else", "return", "sizeof", "printf", "scanf",
    "struct", "main", "void", "include", "return", "define"
};

int is_keyword(const char *word) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;  
        }
    }
    return 0;  
}

void to_uppercase(char *word) {
    for (int i = 0; word[i]; i++) {
        word[i] = toupper(word[i]);
    }
}

int main() {
    FILE *fa, *fb;

    fa = fopen("input3.c", "r");
    if (fa == NULL) {
        printf("File cannot be opened\n");
        exit(0);
    }

    fb = fopen("output3.c", "w");
    if (fb == NULL) {
        printf("File cannot be opened\n");
        fclose(fa);
        exit(0);
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), fa)) {
        char word[MAX_LINE_LENGTH];
        int word_index = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            if (isalpha(line[i]) || isdigit(line[i])) {  
                word[word_index++] = line[i];
            } else {
                if (word_index > 0) {
                    word[word_index] = '\0';  
                    if (is_keyword(word)) {
                        to_uppercase(word);  
                    }
                    fputs(word, fb);  
                    word_index = 0;  
                }
                fputc(line[i], fb);  
            }
        }
    }

    fclose(fa);
    fclose(fb);

    printf("The C program has been processed and saved with keywords in uppercase.\n");

    return 0;
}
