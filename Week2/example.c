#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fa, *fb;  // Correct declaration
    int ca, cb;

    fa = fopen("q4.txt", "r");
    if (fa == NULL) {
        printf("File doesn't exist\n");
        exit(0);
    }

    fb = fopen("q4out.txt", "w");

    ca = getc(fa);
    while (ca != EOF) {
        if (ca == ' ') {
            putc(ca, fb);
            while (ca == ' ') {
                ca = getc(fa);
            }
        }

        if (ca == '/') {
            cb = getc(fa);
            if (cb == '/') {  // Single-line comment
                while (ca != '\n') {
                    ca = getc(fa);
                }
            } else if (cb == '*') {  // Multi-line comment
                do {
                    while (ca != '*') {
                        ca = getc(fa);
                    }
                    ca = getc(fa);  // Read past the '*' character
                } while (ca != '/');  // Continue until we find the closing '/'
            } else {
                putc('/', fb);
                putc(cb, fb);
            }
        } else {
            putc(ca, fb);
        }

        ca = getc(fa);
    }

    fclose(fa);
    fclose(fb);

    return 0;
}
