
#include <stdio.h>
#include <stdlib.h>
int main()
{
    FILE *fptr1;    
    char filename[100], c;
    int numc = 0;        // Variable to count the number of characters in the file
    int numl = 0;        // Variable to count the number of lines in the file

    printf("Enter the filename to open for reading: \n");
    scanf("%s", filename); // Read the filename from the user
    // Open the file for reading ("r" mode)

    fptr1 = fopen(filename, "r"); 
    if (fptr1 == NULL) {
        printf("Cannot open file %s \n", filename);
         exit(0); }

    c = fgetc(fptr1); // Read the first character from the file
    
    while (c != EOF) // Continue reading until the end of the file (EOF)
    {
        if (c == '\n') // If the character is a newline (end of a line)
            numl++; // Increment the line counter
        else 
            numc++; // Otherwise, it's a character, so increment the character counter

        // Read the next character
        c = fgetc(fptr1); 
    }

    // Since the last line may not end with a newline, add 1 to the line count
    numl++;
    printf("Number of characters: %d \n", numc);
    printf("Number of lines: %d \n", numl);

    // Close the file after reading
    fclose(fptr1); 

    return 0; 
}