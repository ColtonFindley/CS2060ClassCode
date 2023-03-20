
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define LENGTH 13

bool exploreValidateInt(const char* buff, int* validInt);
bool validateInt(char* buff, int* const validInt);
void printLimits();

int main(void)
{
	char inputStr[LENGTH]; // create char arintray
	size_t inputLength = 0;
	int validInt = 0;

	printLimits();

	for (unsigned int counter = 1; counter < 6; counter++)
	{
		do {
			puts("\nEnter an integer");
			fgets(inputStr, LENGTH, stdin);

			inputLength = strnlen(inputStr, LENGTH);

			if (inputLength > 0 && inputStr[inputLength - 1] == '\n')
			{
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}
		} while (!exploreValidateInt(inputStr, &validInt));
		
		
		
	}

}


void printLimits()
{
	printf("The number of bits in a byte %d\n", CHAR_BIT);

	printf("The minimum value of SIGNED CHAR = %d\n", SCHAR_MIN);
	printf("The maximum value of SIGNED CHAR = %d\n", SCHAR_MAX);
	printf("The maximum value of UNSIGNED CHAR = %d\n", UCHAR_MAX);

	printf("The minimum value of SHORT INT = %d\n", SHRT_MIN);
	printf("The maximum value of SHORT INT = %d\n", SHRT_MAX);

	printf("The minimum value of INT = %d\n", INT_MIN);
	printf("The maximum value of INT = %d\n", INT_MAX);

	printf("The minimum value of CHAR = %d\n", CHAR_MIN);
	printf("The maximum value of CHAR = %d\n", CHAR_MAX);

	printf("The minimum value of LONG = %ld\n", LONG_MIN);
	printf("The maximum value of LONG = %ld\n", LONG_MAX);
}


bool exploreValidateInt(const char* buff, int* validInt)
{
	// Decalre pointer to a char that will be passed to strotol
	char* end;
	// The errno is set to zero at program startup and set to ERANGE if out of range when strtol tires to convert to double
	errno = 0;
	// Takes the integer value in the string and moves it to a long
	long intTest = strtol(buff, &end, 10);
	bool ifValid = false;


	// Checks if the value entered is not a number
	if (end == buff) {
		fprintf(stderr, "%s: not a decimal number\n", buff);
	}
	// If there was non integer values at the end of the string
	else if ('\0' != *end) {
		fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
	}
	// If the string was out of bounds on long length
	else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	// If the string was out of bounds on int length
	else if (intTest > INT_MAX) {
		fprintf(stderr, "%ld greater than INT_MAX\n", intTest);
	}
	// If the string was less than int length
	else if (intTest < INT_MIN) {
		fprintf(stderr, "%ld less than INT_MIN\n", intTest);
	}
	// What was entered was a valid int
	else {
		// Converts long to int
		*validInt = (int)intTest;
		ifValid = true;
		printf("%d is integer value ", intTest);
	}
	return ifValid;
}
