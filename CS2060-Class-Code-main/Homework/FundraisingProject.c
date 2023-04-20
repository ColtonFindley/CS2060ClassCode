/*
* Colton Findley
* This program simulates a donation service for an organization. First the organization
* enters their information. Then the donation screen repeats until the special character
* is entered, where a summary of the donations is displayed
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define PROCESSING_FEE 3.1
#define LENGTH 80
#define BEGINNING "https:donate.com/["
#define END "]?form=popup#"
#define ATTEMPTS 3
typedef struct org {
	char orgName[LENGTH]; // Name of the organization
	char purpose[LENGTH]; // Purpose of the organization
	char userName[LENGTH]; // Name of the organization's user
	double goalAmount[LENGTH]; // Goal amount of the organization
	char email[LENGTH]; // Email of the user
	char password[LENGTH]; // Password of the user
	char url[LENGTH]; // Created URL
} Organization;
void getOrgName(char* orgName); // Gets the organization's name
void getPurpose(char* purpose); // Gets the organization's purpose
void getUserName(char* userName); // Gets the organizations user's name
void getGoalAmount(double* goalAmount); // Gets the goal amount for the organization
bool validateDouble(const char* buff); // Returns true if the integer entered is valid
void getEmail(char* email); // Gets the email of the user of the organization
void getPassword(char* password); // Gets the password of the user of the organization
void createURL(const char orgName[LENGTH], char* url); // Creates the URL for the organization
void displayInformation(double amtRaised, const char url[LENGTH], const char orgName[LENGTH], 
	const char purpose[LENGTH], const char orgGoalAmount[LENGTH]); // Displays the donation information
bool getDonation(double* userDonated, char* email, char* password); // Gets the donation from the donator, return false if donator enters a q or Q
void getDonaterName(char* donaterName); // Gets the donator's name
void zipCode(const double userDonated, double* amtRaised, double* processingFee); // Gets and validates an entered zipcode
void receipt(const char orgName[LENGTH], const double userDonated); // If the donator says y or Y, a receipt of their donation is displayed
bool adminValidation(char* email, char* password); // Returns true if the correct email and password were entered
void endDisplay(const char orgName[LENGTH], const double numDonations,  
	const double amtRaised, const double processingFee); // Displays a summary of the total donations
void newLine(char* string);
bool yOrN();


int main(void) {
	Organization org; // Organization structure
	// Set up the organization
	getOrgName(org.orgName);
	getPurpose(org.purpose);
	getUserName(org.userName);
	getGoalAmount(org.goalAmount);
	getEmail(org.email);
	getPassword(org.password);
	createURL(org.orgName, org.url);

	printf("Thank you %s. The URL to raise funds for %s is %s", org.userName, org.orgName, org.url);

	double amtRaised = 0; // Total amount of money raised
	double userDonated = 0; // What the user donated before the processing fee
	char donaterName[LENGTH]; // Name of the donator
	double totalProcessingFee = 0; // Total amount of money from the processing fee
	unsigned int donationCounter = 0; // Number of donations
	displayInformation(amtRaised, org.url, org.orgName, org.purpose, org.goalAmount);

	// Loops until the admin enters a q or Q, and the email and password are valid
	while (getDonation(&userDonated, org.email, org.password)) {
		donationCounter++;
		// Donator information
		getDonaterName(&donaterName);
		zipCode(userDonated, &amtRaised, &totalProcessingFee);
		receipt(org.orgName, userDonated);
		displayInformation(amtRaised, org.url, org.orgName, org.purpose, org.goalAmount);
	}
	endDisplay(org.orgName, donationCounter, amtRaised, totalProcessingFee);
}

// Has the user input the name of the organization
void getOrgName(char* orgName) {
	char userOrgName[LENGTH]; // What the user enters

	puts("Enter fundraising organization name");
	fgets(userOrgName, LENGTH, stdin);
	
	// Getting rid of the new line
	newLine(userOrgName);
	strncpy(orgName, userOrgName, LENGTH);
}

// Has the user input the purpose of the organization
void getPurpose(char* purpose) {
	char userPurpose[LENGTH]; // What the user enters

	puts("\nEnter fundraiser purpose\n");
	fgets(userPurpose, LENGTH, stdin);

	// Getting rid of the new line
	newLine(userPurpose);
	strncpy(purpose, userPurpose, LENGTH);
}

// Has the user enter their first and last name
void getUserName(char* userName) {
	char userNameInput[LENGTH]; // What the user enters

	puts("\nEnter first and last name\n");
	fgets(userNameInput, LENGTH, stdin);
	
	// Getting rid of the new line
	newLine(userNameInput);
	strncpy(userName, userNameInput, LENGTH);
}

// Has the user enter the goal amount of donations
void getGoalAmount(double* goalAmount) {
	char inputStr[LENGTH]; // What the user enters

	// Loops until the user enters a valid number
		do {
			puts("Enter the goal amount you want to raise\n");
			fgets(inputStr, LENGTH, stdin);

			// Getting rid of the new line
			newLine(inputStr);
		} while (!validateDouble(inputStr));

		strncpy(goalAmount, inputStr, LENGTH);
	
}

// Validates the user's amount input
bool validateDouble(const char* buff) {
	// Decalre pointer to a char that will be passed to strotol
	char* end;
	// The errno is set to zero at program startup and set to ERANGE if out of range when strtol tires to convert to double
	errno = 0;
	// Takes the integer value in the string and moves it to a long
	double intTest = strtod(buff, &end);
	bool ifValid = false; // Return value

	// Checks if the value entered is not a number
	if (end == buff) {
		fprintf(stderr, "%s: not a decimal number\n", buff);
	}
	// If there was non integer values at the end of the string
	else if ('\0' != *end && '.' != *end) {
		fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
	}
	// If the string was out of bounds on long length
	else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) {
		fprintf(stderr, "%s out of range of type long\n", buff);
	}
	// If the string was out of bounds on int length
	else if (intTest > INT_MAX) {
		fprintf(stderr, "%lf greater than INT_MAX\n", intTest);
	}
	// If the string was less than int length
	else if (intTest < INT_MIN) {
		fprintf(stderr, "%lf less than INT_MIN\n", intTest);
	}
	// If the string was less than 0
	else if (intTest <= 0) {
		puts("Number must be greater than 0");
	}
	// What was entered was a valid int
	else {
		ifValid = true;
	}
	return ifValid;
}

// Has the user create an email for the fundraiser
void getEmail(char* email) {
	char userEmail[LENGTH]; // What the user enters for their email
	bool emailLoop = true; // Expression to exit the validation loop

	// Loops until the email passes all checks
	while (emailLoop) {
		puts("\nEnter email address");
		fgets(userEmail, LENGTH, stdin);

		// Getting rid of the new lien
		newLine(userEmail);

		// Asks user if email is correct
		printf("\nIs this email correct (y)es or (n)o?: %s\n", userEmail);
		emailLoop = yOrN();
	}
	strncpy(email, userEmail, LENGTH);
}

// Has the user enter their password
void getPassword(char* password) {
	char userPasswordInput[LENGTH]; // User entered password

	puts("\nEnter password");
	fgets(userPasswordInput, LENGTH, stdin);
	
	// Getting rid of the new line
	newLine(userPasswordInput);
	strncpy(password, userPasswordInput, LENGTH);
}

// Creates the url based on what the user inputed
void createURL(const char orgName[LENGTH], char* url) {
	char tempOrgName[80]; // Temporary organization name
	strncpy(tempOrgName, orgName, LENGTH);
	char* bufferPtr; // Char for strchr return

	// Checks for spaces in the organizations's name
	bufferPtr = strchr(tempOrgName, ' ');
	while (bufferPtr != NULL) {
		// Changes the address of a space to a -
		// Then moves the pointer along to find another space
		*bufferPtr = '-';
		bufferPtr++;
		bufferPtr = strchr(tempOrgName, ' ');
	} 

	char tempChar[LENGTH]; // For making the URL
	// Takes the three parts of the URL and combines them
	strncpy(tempChar, BEGINNING, LENGTH);
	strcat(tempChar, tempOrgName);
	strcat(tempChar, END);

	int test = 0; // For iterating
	// Loops through the URL and makes all letters lowercase
	while ( test < strlen(tempChar)) {
		tempChar[test] = tolower(tempChar[test]);
		test++;
	}
	strncpy(url, tempChar, LENGTH);
	
}

// Displays the user's organization information
void displayInformation(double amtRaised, const char url[LENGTH], const char orgName[LENGTH], 
	const char purpose[LENGTH], const char orgGoalAmount[LENGTH]) {
	printf("\n\n%s", url);
	puts("\nMAKE A DIFFERENCE BY YOUR DONATION");
	printf("Organization: %s", orgName);
	printf("\nPurpose: %s", purpose);
	printf("\nWe have currently raised $%.1lf", amtRaised);

	char* end; // Char pointer for strtod
	double goalAmount = strtod(orgGoalAmount, &end); // String goalAmount converted into a double
	// If the amount raised is greater than the goal amount
	if (amtRaised >= goalAmount) {
		puts("\nWe have reached our goal but could still use donations");
	}
	// If the amount raised is less than the goal amount
	else {
		double percentage = amtRaised / goalAmount; // Calculates the percentage towards the goal amount
		percentage = percentage * 100;
		printf("\nWe are %.1lf percent towards our goal of $%.1lf", percentage, goalAmount);
	}
}

// Gets the user's donation, and adds it to the total amount raised
bool getDonation(double* userDonated, char* email, char* password) {
	char inputStr[LENGTH]; // What the user inputs
	bool tester = false; // If what the number entered is valid
	char* end; // Char pointer for strtod
	bool adminTest = true; // If the user enters q or Q and passes the checks

	// Loops if admin code is not entered and the donation is valid
	do {
		// Loops until the admin code is not entered and the donation is valid
		while (adminTest && !tester) {
			puts("\nEnter the amount you want to donate\n");
			fgets(inputStr, LENGTH, stdin);

			// Getting rid of the new line
			newLine(inputStr);

			char test = inputStr[0]; // First character entered

			// If the admin code was entered
			if (test == 'q' || test == 'Q') {
				// If the donator passes the chekcs
				if (adminValidation(email, password)) {
					adminTest = false;
				}
			}
			// If a regular donation is entered
			else {
				tester = validateDouble(inputStr);
			}
		}
	} while (adminTest && !tester);

	// Updates what the user donated
	*userDonated = strtod(inputStr, &end);
	return adminTest;
}

// Gets the donater's name
void getDonaterName(char* donaterName) {
	char userName[LENGTH]; // What was entered

	puts("\nEnter first and last name\n");
	fgets(userName, LENGTH, stdin);

	// Getting rid of the new line
	newLine(userName);
	strncpy(donaterName, userName, LENGTH);
}

// Asks the user for a zipcode and checks if it was valid
void zipCode(const double userDonated, double* amtRaised, double* processingFee) {
	char zipCode[LENGTH]; // What was entered
	size_t inputLength = 0; // For the new line
	bool valid = true; // Expression if zipcode is invalid
	do {
		valid = true;
		int count = 0; // Counter for if zipcode passed all the checks

		puts("Enter your 5 digit zipcode");
		fgets(zipCode, LENGTH, stdin);

		// Getting rid of the new line
		newLine(zipCode);

		// If the length is not 5
		inputLength = strnlen(zipCode, LENGTH);
		if (inputLength < 5 || inputLength > 5) {
			puts("Zipcode needs to be 5 numbers");
			valid = false;
			count++;
		}
		// If the first number is 0
		else if (valid && zipCode[0] == '0') {
			puts("Zipcode cannot begin with the number 0");
			valid = false;
			count++;
		} 
		// If the zipcode is not a valid double
		else if  (valid && !validateDouble(zipCode)) {
			valid = false;
			count++;
		}
		// If the zipcode passed all the checks
		else if (count == 0) {
			valid = true;

			// Ending zipcode display
			printf("\nThere is a %.1lf percent credit card processing fee of %.1lf. ",
				PROCESSING_FEE, userDonated);
			double tempProcessingFee = userDonated * (double)(PROCESSING_FEE / 100); // Temporary proccessing fee
			// Adds to the total processing fee
			*processingFee = *processingFee + tempProcessingFee;
			double tempUserDonated = userDonated - tempProcessingFee; // Temporary value for what the user donated
			// Adds to the total amount raised
			*amtRaised = *amtRaised + tempUserDonated;
			printf("%.1lf will be donated", tempUserDonated);
		}
	} while (!valid);

}

// Promts the user if they want a receipt
// If yes, displays the donation information
void receipt(const char orgName[LENGTH], const double userDonated) {
	bool test = true; // Boolean flag
	
	puts("\nDo you want a receipt (y)es or (n)o?");
	test = yOrN();

	if (!test) {
		printf("Organization: %s", orgName);
		printf("\nDonation Amount ($): %.1lf", userDonated);

		time_t now;
		time(&now);
		struct tm* local = localtime(&now);
		printf("\nTime is: %d/%d/%d %d:%d:%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
			local->tm_hour - 12, local->tm_min, local->tm_sec);
	}

}

// Checks if the email and password are correct
// If correct, ends the donating loop
bool adminValidation(char* email, char* password) {
	bool returnValue = true; // Return value
	bool emailTest = true; // Expression for the email checks
	bool passwordTest = true; // Expression for the password checks
	unsigned int emailCount = 0; // Counter for failed email attemps
	unsigned int passwordCount = 0; // Counter for failed password attempts
	char userEmail[LENGTH]; // What was entered for email
	char userPassword[LENGTH]; // What was entered for password

	// For the email checks
	while (emailTest) {
		puts("Enter your email");
		fgets(userEmail, LENGTH, stdin);

		// Getting rid of the new line
		newLine(userEmail);

		int compare = strcmp(userEmail, email); // If the two emails are the same
		// 0 if they are the same
		if (compare == 0) {
			emailTest = false;
		}
		// The emails are not the same
		else {
			emailCount++;
			// If two incorrect attempts have happened
			if (emailCount == ATTEMPTS) {
				emailTest = false;
				returnValue = false;
			}
		}
	}
	// For the password checks, if the email was valid, and the return value is true
	while (returnValue && !emailTest && passwordTest) {
		puts("Enter your password");
		fgets(userPassword, LENGTH, stdin);

		// Getting rid of the new line
		newLine(userPassword);

		int compare = strcmp(userPassword, password); // If the two passwords are the same
		// 0 if the passwords are the same
		if (compare == 0) {
			passwordTest = false;
		}
		// If the passwords were not equal
		else {
			passwordCount++;
			// If two incorrect attemps have happened
			if (passwordCount == ATTEMPTS) {
				passwordTest = false;
				returnValue = false;
			}
		}
	}
	return returnValue;
}

// Displays the donation summary
void endDisplay(const char orgName[LENGTH], const double numDonations,
	const double amtRaised, const double processingFee) {
	puts("\nDonation Summary:");
	printf("Organization Name: %s", orgName);
	printf("\nTotal Number of donations: %.1lf", numDonations);
	printf("\nTotal amount raised: $%.1lf", amtRaised);
	printf("\nTotal amount paid for credit card processing: $%.1lf", processingFee);
}

// Gets rid of the new line
void newLine(char* string) {
	size_t inputLength = 0; // For the new line

	// Getting rif of the new line
	inputLength = strnlen(string, LENGTH);
	if (inputLength > 0 && string[inputLength - 1] == '\n') {
		string[inputLength - 1] = '\0';
		inputLength--;
	}
}

// Asks the user a y or n question
bool yOrN() {
	char yesOrNo; // If the user wants to enter a pet
	bool tester = false; // If y or n was not entered
	bool returnValue;

	do {
		tester = false;
		// Gets input from user
		yesOrNo = getchar();
		// Clears the buffer
		while (getchar() != '\n');
		// Letter to lowercase
		yesOrNo = tolower(yesOrNo);
		// If user enters yes
		if (yesOrNo == 'y') {
			returnValue = false;
		}
		// If user enters no
		else if (yesOrNo == 'n') {
			returnValue = true;
		}
		// If yes or no was not entered
		else {
			puts("(Y)es or (n)o was not entered");
			tester = true;
		}
	} while (tester);
	return returnValue;
}