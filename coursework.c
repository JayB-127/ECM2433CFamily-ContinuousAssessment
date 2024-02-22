#include <coursework.h>

int main (int argc, char **argv) {
	
	/*TASK 1*/
	printf("--- TASK 1 ---\n");
	/*Example 1*/
	unsigned char  var1[] = {10, 20, 30, 145};
	void *ptr1 = &var1;
	printBytes(ptr1, 4);
	/*Expected output:
	*Starting at memory address XXXX:
	*001:   10
	*002:   20
	*003:   30
	*004:  145
	*/

	/*Example 2*/
	long int var2 = 214748364623; /*long int will be stored as bytes of its binary representation, starting with the least significant*/
	void *ptr2 = &var2;
	printBytes(ptr2, 8);
	/*Expected output:
	*Starting at memory address XXXX:
	*001:   79
	*002:  255
	*003:  255
	*004:  255
	*005:   49
	*006:    0
	*007:    0
	*008:    0
	*/

	/*TASK 2*/
	printf("--- TASK 2 ---\n");
	reverseFile(argv[1], argv[2]); /*argv[1] and argv[2] are the two command line inputs (filenames for input and output files respectively).*/
	printf("Reversed \"%s\", outputted to \"%s\".\n", argv[1], argv[2]);

	/*TASK 3*/
	printf("--- TASK 3 ---\n");
	msStringTest();

	return 0;
}

/*Function for Task 1*/
extern void printBytes(void* ptr, int numBytes) {

	/*output address currently pointed to by ptr*/
	printf("Starting at memory address %p:\n", ptr);
	
	/*repeat for desired amount of bytes*/
	int i = 1;
	while (i < numBytes+1) {
		/*print what is currently stored in 'byte i'*/
		printf("%03d: %4d\n", i, *(unsigned char *)ptr);
		/*increment ptr by 1 byte*/
		ptr = (unsigned char *)ptr + 1;
		i++;
	}
}

/*Function for Task 2*/
extern void reverseFile(char* fileInName, char* fileOutName) {
	
	/*open fileIn, trapping an error if requried*/
	FILE *fileIn;
	if ((fileIn = fopen(fileInName, "r")) == NULL) {
		fprintf(stderr, "Cannot open %s for read access: %s\n", fileInName, strerror(errno));
		exit(1);
	}

	/*open fileOut, trapping an error if required*/
	FILE *fileOut;
	if ((fileOut = fopen(fileOutName, "w")) == NULL) {
		fprintf(stderr, "Cannot open %s for write access: %s\n", fileOutName, strerror(errno));
		exit(1);
	}

	/*get size of fileIn*/
	fseek(fileIn, 0, SEEK_END);
	long int sizeIn = ftell(fileIn);
	long int sizeOut;

	/*read fileIn, char by char, in reverse, writing each char to fileOut*/
	char c;
	int i = -2;
	while(1) {
		/*read the char that is positioned at the 'ith' position from the end of fileIn and write it to fileOut*/
		fseek(fileIn, i, SEEK_END);
		c = fgetc(fileIn);
		fputc(c, fileOut);
		i--;
		/*get size of fileOut*/
		fseek(fileOut, 0, SEEK_END);
		sizeOut = ftell(fileOut);
		/*if size of files are equal then reversing is done, so break out of loop*/
		if (sizeIn == sizeOut + 1) { /*+1 accounts for the skipped /n in fileIn*/
			break;
		}
	}

	/*add back newline character that was skipped from fileIn*/
	fputs("\n", fileOut);

	/*close files*/
	fclose(fileIn);
	fclose(fileOut);
}

/*Function for Task 3*/
extern void msStringTest() {

	/*Test code for Task 3*/
	msString ms = msSetString("Hello");
	msString ms2 = msSetString(" World!");
	msString mscopy = NULL;

	char *msStr = msGetString(ms); /*store return of msGetString(ms) so that memory allocation can be freed*/
	printf("String |%s| is %d characters long (%p).\n", msStr, msLength(ms), ms);

	msCopy(&mscopy,ms);
	char *mscopyStr = msGetString(mscopy); /*store return of msGetString(mscopy) so that memory allocation can be freed*/
	printf("Copied string |%s| is %d characters long (%p).\n", mscopyStr, msLength(mscopy), mscopy);

	printf("Compare ms with mscopy: %d\n", msCompare(ms, mscopy));
	printf("Compare ms with ms2   : %d\n", msCompare(ms, ms2));
	printf("Compare ms with Hello : %d\n", msCompareString(ms, "Hello"));
	printf("Compare ms with HelloX: %d\n", msCompareString(ms, "HelloX"));
	printf("Compare ms with Hella : %d\n", msCompareString(ms, "Hella"));

	msConcatenate(&mscopy,ms2);
	free(mscopyStr); /*Free memory allocation to prevent memory leak*/
	mscopyStr = msGetString(mscopy);
	printf("Concatenated string |%s| is %d characters long (%p).\n", mscopyStr, msLength(mscopy), mscopy);

	/*Free all memory allocations, in order to prevent memory leaks*/
	free(ms);
	free(msStr);
	free(ms2);
	free(mscopy);
	free(mscopyStr);

	/*MEMORY LEAK FIX:
	* The above code has been adapted from the given test code to include variables that store the return of msGetString(msString).
	* This is necessary to allow memory, that was allocated within the msGetString() function, to be freed, avoiding memory leaks.
	* In addition, all msStrings are freed at the end to avoid memory leaks.
	* Where these additions have been made, I have placed comments to highlight it.*/
}
