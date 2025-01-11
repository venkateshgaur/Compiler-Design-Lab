#include <stdlib.h>
#include <stdio.h>

int main() {
    FILE *fp1, *fp2, *fp3;
    char filename[100];
    char c1, c2;

    printf("Enter file1: ");
    scanf("%99s", filename);  // Safe input
    fp1 = fopen(filename, "r");
    if (fp1 == NULL) {
        printf("Error opening file1.\n");
        return 1;
    }

    printf("Enter file2: ");
    scanf("%99s", filename);  // Safe input
    fp2 = fopen(filename, "r");
    if (fp2 == NULL) {
        printf("Error opening file2.\n");
        return 1;
    }

    printf("Enter file3: ");
    scanf("%99s", filename);  // Safe input
    fp3 = fopen(filename, "w+");
    if (fp3 == NULL) {
        printf("Error opening file3.\n");
        return 1;
    }

    // Read characters from both files and alternate writing to fp3
    c1 = fgetc(fp1);
    c2 = fgetc(fp2);

    while (c1 != EOF && c2 != EOF) {
        // Write characters from file1
        while (c1 != '\n' && c1 != EOF) {
            fputc(c1, fp3);
            c1 = fgetc(fp1);  // Update c1 after writing
        }

        // Write characters from file2
        while (c2 != '\n' && c2 != EOF) {
            fputc(c2, fp3);
            c2 = fgetc(fp2);  // Update c2 after writing
        }

        if (c1 == '\n') {
            c1 = fgetc(fp1);
        }
        if (c2 == '\n') {
            c2 = fgetc(fp2);
        }

        fputc('\n', fp3);
    }

    while (c1 != EOF) {
        // Write characters from file1
        while (c1 != '\n' && c1 != EOF) {
            fputc(c1, fp3);
            c1 = fgetc(fp1);  // Update c1 after writing
        }

        if (c1 == '\n') {
            c1 = fgetc(fp1);
        }

        fputc('\n', fp3);
    }

    while (c2 != EOF) {

        // Write characters from file2
        while (c2 != '\n' && c2 != EOF) {
            fputc(c2, fp3);
            c2 = fgetc(fp2);  // Update c2 after writing
        }
        
        if (c2 == '\n') {
            c2 = fgetc(fp2);
        }

        fputc('\n', fp3);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    printf("\nResult written to file3\n");
    return 0;
}