// Colton Findley, HW02
// This code will simulate a parking garage, with how many cars and what the fees are

#include <stdio.h>
#define MINIMUM_FEE 2.00 // Beginning fee
#define HOUR_FEE 0.75 // Fee for every hour over the minimum
#define MAXIMUM_CHARGE 10.00 // Largest charge available
#define FREE_HOUR 3 // Hours that get no extra charge
double calculations(double charge); // Calculates the charge
void display(int count, double hours, double charge); // Displays cars, hour, and charge
void endDisplay(int count, double totalHours, double totalCharge); // Displays the total cars, hours, and charge

int main(void) {

	double hours = 0; // User entered hours
	unsigned int validNumber = 0; // If user entered a valid number
	unsigned int count = 0; // Number of cars
	double charge = 0; // How much money the user is charged
	double totalCharge = 0; // Total amount of charges
	double totalHours = 0; // Total amount of hours

	puts("Enter the number of hours the car was parked or enter -1 to quit.");
	validNumber = scanf("%lf", &hours); // Asking user for a double 
	while ((getchar()) != '\n'); // Clearing the buffer

	// If hours was less than 0 or more than 24, excluding -1
	while (hours == 0 || hours != -1 && hours < -1 || hours > 24) {
		puts("Invalid");
		validNumber = scanf("%lf", &hours); // Asking user for a double 
		while ((getchar()) != '\n'); // Clearing the buffer
	}

	// Loop untill the user enters a -1
	while (hours != -1) {

		// If user entered a valid number
		if (validNumber == 1) {

			charge = calculations(hours); // Calculating the charge
			totalCharge = totalCharge + charge;
			totalHours = totalHours + hours;
			
			count++;
			display(count, hours, charge);
			
			// Prompts the user for another input
			puts("Enter the number of hours the car was parked or enter -1 to quit.");
			validNumber = scanf("%lf", &hours); // Asking user for a double
			while ((getchar()) != '\n'); // Clearing the buffer
		}
		// If user entered an invalid number
		else if (validNumber == 0) {
			puts("You did not enter a number");

			// Prompting the user for another input
			puts("Enter the number of hours the car was parked or enter -1 to quit.");
			validNumber = scanf("%lf", &hours); // Asking user for a double
			while ((getchar()) != '\n'); // Clearing the buffer
		}
	}
	endDisplay(count, totalHours, totalCharge);

} // End main

double calculations(double charge) {
	
	double finalCharge = MINIMUM_FEE;
	charge = charge - FREE_HOUR;

	// Loop for adding the additional charge
	while (charge > 0) {
		finalCharge = finalCharge + HOUR_FEE;
		charge--;
	}
	// If charge is over the maximum
	if (finalCharge > MAXIMUM_CHARGE) {
		finalCharge = MAXIMUM_CHARGE;
	}

	return finalCharge;

} // calculations

void display(int count, double hours, double charge) {
	puts("Car          Hours         Charge");
	printf("%-12d %-13.1f %.2f\n", count, hours, charge);
} // display

void endDisplay(int count, double totalHours, double totalCharge) {
	puts("Parking Garage Summary");
	if (count == 0) {
		puts("There were no cars parked today.");
	}
	else {
		puts("Car          Hours         Charge");
		printf("%-12d %-13.1f %.2f\n", count, totalHours, totalCharge);
	}
} // endDisplay