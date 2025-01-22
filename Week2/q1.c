#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fa, *fb;

    fa = fopen("input.c", "r");
    if (fa == NULL) {
        printf("Cannot open input file\n");
        exit(0);
    }

    fb = fopen("output.c", "w");
    if (fb == NULL) {
        printf("Cannot open output file\n");
        fclose(fa);  
        exit(0);
    }

    int ca;
    int str_lit = 0;
    int char_lit = 0;

    ca = getc(fa);
    while (ca != EOF) {
        if (ca == '"' && !char_lit) {  
            putc(ca, fb);
            ca = getc(fa);
            str_lit = !str_lit;
            continue;
        }

        if (ca == '\'' && !str_lit) {  
            putc(ca, fb);
            ca = getc(fa);
            char_lit = !char_lit;
            continue;
        }

        if (str_lit || char_lit) {  
            putc(ca, fb);
            ca = getc(fa);
        } else {  
            if (ca == ' ' || ca == '\t') {  
                putc(' ', fb);
                do {
                    ca = getc(fa);
                } while (ca == ' ' || ca == '\t');
            } else {
                putc(ca, fb);
                ca = getc(fa);
            }
        }
    }

    printf("The output file output.c has content replacing blank spaces and tabs with a single space from input file input.c\n");

    fclose(fa);  
    fclose(fb);  

    return 0;
}
