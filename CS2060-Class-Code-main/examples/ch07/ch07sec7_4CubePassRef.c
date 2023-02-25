// Passing Primitive Data Types and pointers
//Add comments to explain the code

#include <stdio.h>	
#include <stdlib.h>

int cubePass1(int number);
void cubePass2(int * numberPtr);
void func1(const int* const numberPtr);
void func2(const int* const numberPtr);

int main(void) {

	int numberMain = 5; // Pass by value
	int result = 0; // Return value

	int test = 33;
	int test1 = 3;
	int test2 = 10;
	int test3 = 0;
	int test4 = -2;

	int *array[5] = { &test, &test1, &test2, &test3, &test4 };
	for (int i = 0; i < 5; i++) {
		printf("array[%d} %p\n", i, array[i]);
		printf("*array[%d] %p\n", i, array[i]);
	}
	
	printf ("In main before cubePass1  numberMain = %d\n", numberMain);
	printf("&numberMain = %p\n", &numberMain);
	result = cubePass1(numberMain);
	printf ("In main after cubePass1  numberMain = %d\n", numberMain);
	printf ("Result = %d\n", result);

	printf("\nIn main before cubePass2  numberMain = %d\n", numberMain);
	cubePass2(&numberMain);
	printf("\nIn main after cubePass2  numberMain = %d\n", numberMain);
	printf("result = %d\n", result);

	func1(&numberMain);
	func2(&numberMain);


} // main


int cubePass1 (int number) 
{
	int cube = 0;
	puts("\nIn cubePass1");
	printf("number = %d\n", number); // 5
	printf("&number = %p\n", &number); // Address
	cube = number * number * number; // 5 * 5 * 5
	printf("cube  = %d\n", cube); // Prints 125
	number = cube; // numberMain is not affected outside of cubePass1
	printf("number = %d\n", number); // = cube
	return cube;
} 

void cubePass2(int * const numberPtr) {

	printf("   *numberPtr = %d\n", *numberPtr);

	printf("   numberPtr = %p\n", numberPtr);

	int cube = (*numberPtr) * (*numberPtr) * (*numberPtr);

	*numberPtr = cube;

	*numberPtr = cube;

	printf("   *numberPtr = %d\n\n", *numberPtr);

}

void func1(const int* numberPtr) {
	printf("   *numberPtr = %d\n\n", *numberPtr);
}

void func2(const int* const numberPtr) {
	printf("   *numberPtr = %d\n\n", *numberPtr);
}

