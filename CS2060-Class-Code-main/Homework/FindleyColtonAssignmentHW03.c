// Colton Findley, HW02
// This code will simulate a parking garage, with how many cars and what the fees are

#include <stdio.h>
#define MIN_FLAT_RATE_CHARGE 3.00 // Beginning fee
#define ADDITIONAL_HOURS_RATE 0.75 // Fee for every hour over the minimum
#define  MAX_CHARGE 12.00 // Largest charge available
#define MIN_HOURS_AT_FLAT_RATE 3 // Hours that get no extra charge
#define MAX_HOURS_ALLOWED 24 // Maximum amount of hours
double getValidNumber(double hours, int validNumber);
double calculations(double charge); // Calculates the charge
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

	// Loop untill the user enters a -1
	while (hours != -1) {

		hours = getValidNumber(hours, validNumber);
		validNumber = 1; // When it exits the method, it means the number was valid

		// If user entered a valid number
		if (validNumber == 1 && hours != -1) {

			charge = calculations(hours); // Calculating the charge
			totalCharge = totalCharge + charge;
			totalHours = totalHours + hours;
			
			count++;
			puts("Car          Hours         Charge");
			printf("%-12d %-13.1f $%.2f\n", count, hours, charge);
			
			// Prompts the user for another input
			puts("Enter the number of hours the car was parked or enter -1 to quit.");
			validNumber = scanf("%lf", &hours); // Asking user for a double
			while ((getchar()) != '\n'); // Clearing the buffer
		}
		// If user entered an invalid number
		else if (validNumber == 0 && hours == -1) {
			puts("You did not enter a number");

			// Prompting the user for another input
			puts("Enter the number of hours the car was parked or enter -1 to quit.");
			validNumber = scanf("%lf", &hours); // Asking user for a double
			while ((getchar()) != '\n'); // Clearing the buffer
		}
	}
	endDisplay(count, totalHours, totalCharge);

} // End main

double getValidNumber(double hours, int validNumber) {
	// If hours was less than 0 or more than 24, excluding -1, or valid number = 0
	while (hours == 0 || hours != -1 && hours < -1 || hours > MAX_HOURS_ALLOWED || validNumber == 0) {
		puts("You did not enter a number");
		puts("Enter the number of hours the car was parked or enter -1 to quit.");
		validNumber = scanf("%lf", &hours); // Asking user for a double 
		while ((getchar()) != '\n'); // Clearing the buffer
	}
	return hours;
}

double calculations(double charge) {
	
	double finalCharge = MIN_FLAT_RATE_CHARGE;
	charge = charge - MIN_HOURS_AT_FLAT_RATE;

	// Loop for adding the additional charge
	while (charge > 0) {
		finalCharge = finalCharge + ADDITIONAL_HOURS_RATE;
		charge--;
	}
	// If charge is over the maximum
	if (finalCharge > MAX_CHARGE) {
		finalCharge = MAX_CHARGE;
	}

	return finalCharge;

} // calculations

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