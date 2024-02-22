#include <msString.h>

/*msSetString(char *): accepts a standard C string as an input and returns a new msString*/
extern msString msSetString(char *str) {
	/*checks if msString is NULL*/
	if (str == NULL) {
		msError("NULL string given.");
	}

	long int strLen = strlen(str); /*gets length of string*/
	msString ms; /*creates new msString*/

	/*dynamically allocates enough memory to msString to store all characters plus a long int for the length*/
	if (!(ms = malloc(sizeof(long int) + strLen))) {
		msError("Not enough memory.");
	}

	*(long int *)ms = strLen; /*assign the length of the string to the long int*/
	/*copy all characters from the string into the msString*/
	memcpy(ms + sizeof(long int), str, strLen);

	return ms;
}

/*msGetString(msString): accepts an msString as an input and returns a standard C string*/
extern char *msGetString(msString ms) {
	/*checks if msString is NULL*/
	if (ms == NULL) {
		msError("NULL msString given.");
	}

	long int strLen = msLength(ms); /*gets length of msString*/
	char *str;

	/*dynamically allocates enough memory to store the characters from msString*/
	if (!(str = malloc(strLen + 1))) {
		msError("Not enough memory.");
	}

	/*copy all characters from msString into string*/
	memcpy(str, ms + sizeof(long int), strLen);
	*(str + strLen) = '\0'; /*add null character on the end of the newly made string*/

	return str;
}

/*msCopy(msString *, msString): accepts two inputs, a pointer to an msString (the destination)
 *and an msString (the source), and copies the source value to the destination*/
extern void msCopy(msString *msptr, msString ms) {
	/*checks if msString is NULL*/
	if (ms == NULL) {
		msError("NULL msString given.");
	}

	long int strLen = msLength(ms); /*gets length of msString*/

	/*dynamically allocates enough memory to the destination pointer to store the bytes from the source*/
	if (!(*msptr = malloc(sizeof(long int) + strLen))) {
		msError("Not enough memory.");
	}

	/*copy all bytes from the source msString to the newly allocated memory*/
	memcpy(*msptr, ms, sizeof(long int) + strLen);
}

/*msConcatenate(msString *, msString): accepts two inputs, a pointer to an msString (the destination)
 *and an msString (the source), and concatenates the source value onto the end of the destination*/
extern void msConcatenate(msString *msptr, msString ms) {
	/*checks if msString is NULL*/
	if (ms == NULL) {
		msError("NULL msString given.");
	}

	/*gets length of both msStrings, adding to get the required length of the concatenated msString*/
	long int srcLen = msLength(ms);
	long int destLen = msLength(*msptr);
	long int newLen = srcLen + destLen;

	/*dynamically reallocates enough memoryi for the destination pointer to store the concatenated characters*/
	if (!(*msptr = realloc(*msptr, sizeof(long int) + newLen))) {
		msError("Not enough memory.");
	}

	/*ensure the destination's long int holds the correct length of the msString after concatenation*/
	*(long int *)*msptr = newLen;
	/*copy all characters from the source msString to the newly allocated memory after the destination's final character*/
	memcpy((char *)*msptr + sizeof(long int) + destLen, ms + sizeof(long int), srcLen);
}

/*msLength(msString): accepts an msString as an input and returns an integer value that is the
 *number of characters in the string*/
extern long int msLength(msString ms) {
	/*checks if msString is NULL*/
	if (ms == NULL) {
		msError("NULL msString given.");
	}

	return *(long int *)ms; /*returns the long int at the beginning of the msString which holds the length*/
}

/*msCompare(msString, msString): accepts two msString values as inputs and returns 0 (zero) if
 *they are identical, or 1 if they are not*/
extern int msCompare(msString ms1, msString ms2) {
	/*checks if either msString is NULL*/
	if (ms1 == NULL || ms2 == NULL) {
		msError("NULL msString(s) given.");
	}

	/*get length of both msStrings*/
	long int ms1Len = msLength(ms1);
	long int ms2Len = msLength(ms2);

	if (ms1Len != ms2Len) {
		return 1;
	}

	/*point to the first character in both msStrings (first byte after the long int that holds the length*/
	ms1 = (char *)ms1 + sizeof(long int);
	ms2 = (char *)ms2 + sizeof(long int);

	/*repeat for each char*/
	int i = 0;
	while (i < ms1Len) { /*can use either ms1Len or ms2Len, since we know they are equal after the above if statement*/
		/*if the 'ith' char in both msStrings are not equal, we know they are not identical*/
		if (*((char *)ms1 + i) != *((char *)ms2 + i)) {
			return 1;
		}
		i++;
	}

	return 0;
}

/*msCompareString(msString, char *): accepts an msString value and a standard C string value as inputs
 *and returns 0 (zero) if the characters making up the strings are the same, or 1 if they are not*/
extern int msCompareString(msString ms, char *str) {
	/*checks if either (or both) the msString and string are NULL*/
	if (ms == NULL || str == NULL) {
		msError("NULL msString and string given.");
	} else if (ms == NULL) {
		msError("NULL msString given.");
	} else if (str == NULL) {
		msError("NULL string given.");
	}

	/*get length of both msString and string*/
	long int msLen = msLength(ms);
	long int strLen = strlen(str);

	/*if the lengths of ms and str are not equal, then we know they are not the same*/
	if (msLen != strLen) {
		return 1;
	}

	ms = (char *)ms + sizeof(long int); /*point to the first character in ms (first byte after the long int that holds the length*/

	/*repeat for each char*/
	int i = 0;
	while (i < msLen) { /*we can use either msLen or strLen, since we know they are equal after the above if statement*/
		/*if the 'ith' char in both ms and str are not equal, we know ms and str do not contain the same characters*/
		if (*((char *)ms + i) != str[i]) {
			return 1;
		}
		i++;
	}

	return 0;
}

/*msError(char *): accepts a standard C string as input, outputs it as an error message to the user and then
 *exits the program. This function can be called by any of the other msString functions when an error occurs*/
static void msError(char *str) {
	fprintf(stderr, "Error: %s\n", str); /*outputs trapped error to stderr stream*/
	exit(1);
}
