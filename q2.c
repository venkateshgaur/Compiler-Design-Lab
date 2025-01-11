

#include <stdio.h>
#include <stdlib.h> // For exit()

int main() {
    FILE *fptr1, *fptr2;
    char filename[100];
    long fileSize;
    int c;

    //Enter the filename of the source file
    printf("Enter the filename to open for reading: \n");
    scanf("%s", filename);

    // Open the source file for reading
    fptr1 = fopen(filename, "r");
    if (fptr1 == NULL) {
        printf("Cannot open file %s \n", filename);
        exit(0);
    }

    // Get the file size
    fseek(fptr1, 0, SEEK_END);  // Move the file pointer to the end of the file
    fileSize = ftell(fptr1);    // Get the current file pointer position (file size)
    fseek(fptr1, 0, SEEK_SET);  // Move the file pointer back to the beginning of the file

    printf("\nContents of the original file (%s):\n", filename);
    fseek(fptr1, 0, SEEK_SET); 
    while ((c = fgetc(fptr1)) != EOF) {
        putchar(c);
    }
    printf("\nEnter the filename to open for writing: \n");
    scanf("%s", filename);

    // Open the destination file for writing
    fptr2 = fopen(filename, "w");
    if (fptr2 == NULL) {
        printf("Cannot open file %s \n", filename);
        fclose(fptr1);
        exit(0);
    }

    // Reverse the contents of the file
    for (long i = fileSize - 1; i >= 0; i--) {
        fseek(fptr1, i, SEEK_SET); // Move file pointer to the i-th byte
        c = fgetc(fptr1);         // Read the character at this position
        fputc(c, fptr2);          // Write the character to the destination file
    }

    fclose(fptr2);
    printf("Contents reversed and copied to %s\n", filename);
    fptr2 = fopen(filename, "r");

    printf("\nContents of the reversed file (%s):\n", filename);
    while ((c = fgetc(fptr2)) != EOF) {
        putchar(c);  
    }

    fclose(fptr1);
    fclose(fptr2);

    return 0;
}
