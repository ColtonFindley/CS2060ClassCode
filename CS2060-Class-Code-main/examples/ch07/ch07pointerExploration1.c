/*******************
 Understanding pointers

 Add comments to explain the code
**************/

#include <stdio.h>


int main ()
{
	// Initalize houseNum
	int houseNum = 13; // Int value of 13
	int calcHouseNum1 = 0;
	int calcHouseNum2 = 0;
	int *houseNumPtr = &houseNum; // Pointer to houseNum
	
	printf("houseNum %d\n", houseNum); // Prints int value

	printf("&houseNum %p\n\n", &houseNum); // Prints the address

	printf ("*houseNumPtr  %d\n", *houseNumPtr); // Print 13

	printf ("houseNumPtr %p\n\n", houseNumPtr); // Pointers store the address of another value

	printf ("&houseNumPtr %p\n\n", &houseNumPtr); // Pointers have a seperate address

	calcHouseNum1 =  *houseNumPtr + *houseNumPtr; // The * makes it a value

	printf("*houseNumPtr + *houseNumPtr is %d\n\n", calcHouseNum1);

	calcHouseNum2 = 2 * (*houseNumPtr); // *houseNumPtr = 13

	printf("2 * (*houseNumPtr) is %d\n\n", calcHouseNum2);

	printf("\n&*houseNumPtr = %p"
		"\n*&houseNumPtr = %p\n", &*houseNumPtr, *&houseNumPtr); // Both do the same thing
			
	return 0;
}
