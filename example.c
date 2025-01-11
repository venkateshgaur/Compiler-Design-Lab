// Name: Venkatesh Gaur
// Reg No.: 220905089
// Section : C
// Roll No : 15


#include <stdio.h>
#include <stdlib.h>


int main() 


{
	FILE *ftpr1 ,*ftpr2;
	char filename[100],c; 
	printf("Enter the filename to open for reading\n");
	scanf("%s",filename);
	ftpr1 = fopen(filename,"r");

	if(ftpr1 == NULL)
	{
		printf("Cannot open file to open for reading \n");
		exit(0);
	}

	printf("Enter the filename to open for writing: \n");
	scanf("%s", filename);
	ftpr2 = fopen(filename,"w+"); 
	c = fgetc(ftpr1);
	while(c!=EOF)
	{
		fputc(c,ftpr2);
		c = fgetc(ftpr1);
	} 
	printf("\nContents copied to %s", filename);
	fclose(ftpr1);
	fclose(ftpr2);
	return 0; 
}

//Sample Input and Output
//Enter the filename to open for reading: source.txt
//Enter he filename to open for writing: destination.txt
//Contents copied to destination.txt




