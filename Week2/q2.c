#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fa, *fb;
    
    fa = fopen("input2.c", "r");
    if (fa == NULL) {
        printf("File cannot be opened\n");
        exit(0);
    }

    fb = fopen("output2.c", "w");
    if (fb == NULL) {
        printf("File cannot be opened\n");
        fclose(fa);  
        exit(0);
    }

    char line[100];

    while (fgets(line, sizeof(line), fa)) {
        if (line[0] == '#') {
            continue;
        } else {
            fputs(line, fb);
        }
    }

    fclose(fa);
    fclose(fb);

    printf("The output file has content with preprocessor directives removed.\n");

    return 0;
}
