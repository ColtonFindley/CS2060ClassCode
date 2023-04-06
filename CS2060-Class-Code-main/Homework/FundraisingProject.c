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

#define PROCESSING_FEE 2.9
#define LENGTH 80
#define BEGINNING "https:donate.com/["
#define END "]?form=popup#"
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
void getGoalAmount(char* goalAmount); // Gets the goal amount for the organization
bool validateInt(const char* buff); // Returns true if the integer entered is valid
void getEmail(char* email); // Gets the email of the user of the organization
void getPassword(char* password); // Gets the password of the user of the organization
void createURL(const char orgName[LENGTH], char* url); // Creates the URL for the organization
void displayInformation(const Organization org, double amtRaised); // Displays the donation information
bool getDonation(double* userDonated, char* email, char* password); // Gets the donation from the donator, return false if donator enters a q or Q
void getDonaterName(char* donaterName); // Gets the donator's name
void zipCode(const double userDonated, double* amtRaised, double* processingFee); // Gets and validates an entered zipcode
void receipt(const char orgName[LENGTH], const double userDonated); // If the donator says y or Y, a receipt of their donation is displayed
bool adminValidation(char* email, char* password); // Returns true if the correct email and password were entered
void endDisplay(const char orgName[LENGTH], const double numDonations,  
	const double amtRaised, const double processingFee); // Displays a summary of the total donations


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
	displayInformation(org, amtRaised);

	// Loops until the admin enters a q or Q, and the email and password are valid
	while (getDonation(&userDonated, org.email, org.password)) {
		donationCounter++;
		// Donator information
		getDonaterName(&donaterName);
		zipCode(userDonated, &amtRaised, &totalProcessingFee);
		receipt(org.orgName, userDonated);
		displayInformation(org, amtRaised);
	}
	endDisplay(org.orgName, donationCounter, amtRaised, totalProcessingFee);
}

// Has the user input the name of the organization
void getOrgName(char* orgName) {
	char userOrgName[LENGTH]; // What the user enters
	size_t inputLength = 0; // For getting rid of the new line

	puts("Enter fundraising organization name");
	fgets(userOrgName, LENGTH, stdin);
	
	// Getting rid of the new line
	inputLength = strnlen(userOrgName, LENGTH); 
	if (inputLength > 0 && userOrgName[inputLength - 1] == '\n') {
		userOrgName[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(orgName, userOrgName, LENGTH);
}

// Has the user input the purpose of the organization
void getPurpose(char* purpose) {
	char userPurpose[LENGTH]; // What the user enters
	size_t inputLength = 0; // For the new line

	puts("\nEnter fundraiser purpose\n");
	fgets(userPurpose, LENGTH, stdin);

	// Getting rid of the new line
	inputLength = strnlen(userPurpose, LENGTH);
	if (inputLength > 0 && userPurpose[inputLength - 1] == '\n') {
		userPurpose[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(purpose, userPurpose, LENGTH);
}

// Has the user enter their first and last name
void getUserName(char* userName) {
	char userNameInput[LENGTH]; // What the user enters
	size_t inputLength = 0; // For the new line

	puts("\nEnter first and last name\n");
	fgets(userNameInput, LENGTH, stdin);
	
	// Getting rid of the new line
	inputLength = strnlen(userNameInput, LENGTH);
	if (inputLength > 0 && userNameInput[inputLength - 1] == '\n') {
		userNameInput[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(userName, userNameInput, LENGTH);
}

// Has the user enter the goal amount of donations
void getGoalAmount(char* goalAmount) {
	char inputStr[LENGTH]; // What the user enters
	size_t inputLength = 0; // For the new line

	// Loops until the user enters a valid number
		do {
			puts("Enter the goal amount you want to raise\n");
			fgets(inputStr, LENGTH, stdin);

			// Getting rid of the new line
			inputLength = strnlen(inputStr, LENGTH);
			if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}
		} while (!validateInt(inputStr));

		strncpy(goalAmount, inputStr, LENGTH);
	
}

// Validates the user's amount input
bool validateInt(const char* buff) {
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
		fprintf(stderr, "%ld greater than INT_MAX\n", intTest);
	}
	// If the string was less than int length
	else if (intTest < INT_MIN) {
		fprintf(stderr, "%ld less than INT_MIN\n", intTest);
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
	char emailValid[LENGTH]; // What the user enters if the email is correct
	bool emailLoop = true; // Expression to exit the validation loop
	size_t inputLength = 0; // For the new line

	// Loops until the email passes all checks
	while (emailLoop) {
		bool validLoop = true; // Expression for the user to check if the email is valid

		puts("\nEnter email address");
		fgets(userEmail, LENGTH, stdin);

		// Getting rid of the new line
		inputLength = strnlen(userEmail, LENGTH);
		if (inputLength > 0 && userEmail[inputLength - 1] == '\n') {
			userEmail[inputLength - 1] = '\0';
			inputLength--;
		}

		// Loops until the user enters a y, Y, n, or N
		// If y, continues on. If n, user enters email again
		while (validLoop) {
			printf("\nIs this email correct (y)es or (n)o?: %s\n", userEmail);
			fgets(emailValid, LENGTH, stdin);

			// Getting rid of the new line
			inputLength = strnlen(emailValid, LENGTH);
			if (inputLength > 0 && emailValid[inputLength - 1] == '\n') {
				emailValid[inputLength - 1] = '\0';
				inputLength--;
			}

			char* bufferPtr1 = strchr(emailValid, 'y'); // If user entered y
			char* bufferPtr2 = strchr(emailValid, 'Y'); // If user entered Y
			char* bufferPtr3 = strchr(emailValid, 'n'); // If user entered n
			char* bufferPtr4 = strchr(emailValid, 'N'); // If user entered N

			// the bufferPtrs are not NULL if user entered a y,Y,n or N
			if (bufferPtr1 != NULL || bufferPtr2 != NULL || bufferPtr3 != NULL || bufferPtr4 != NULL) {
				// Exits the loop asking the user if the email is correct
				validLoop = false;
				// If user entered a y or Y
				if (bufferPtr1 != NULL || bufferPtr2 != NULL) {
					// Exits the loop checking if the email passed all checks
					emailLoop = false;
				}
			}
			else {
				puts("You did not enter a y or n");
			}

		} // test loop
	}
	strncpy(email, userEmail, LENGTH);
}

// Has the user enter their password
void getPassword(char* password) {
	char userPasswordInput[LENGTH]; // User entered password
	size_t inputLength = 0; // For the new line

	puts("\nEnter password");
	fgets(userPasswordInput, LENGTH, stdin);
	
	// Getting rid of the new line
	inputLength = strnlen(userPasswordInput, LENGTH);
	if (inputLength > 0 && userPasswordInput[inputLength - 1] == '\n') {
		userPasswordInput[inputLength - 1] = '\0';
		inputLength--;
	}
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
void displayInformation(const Organization org, double amtRaised) {
	printf("\n\n%s", org.url);
	puts("\nMAKE A DIFFERENCE BY YOUR DONATION");
	printf("Organization: %s", org.orgName);
	printf("\nPurpose: %s", org.purpose);
	printf("\nWe have currently raised $%.1lf", amtRaised);

	char* end; // Char pointer for strtod
	double goalAmount = strtod(org.goalAmount, &end); // String goalAmount converted into a double
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
	size_t inputLength = 0; // For the new line
	bool tester = true; // If what the number entered is valid
	char* end; // Char pointer for strtod
	bool adminTest = true; // If the user enters q or Q and passes the checks

	// Loops if admin code is not entered and the donation is valid
	do {
		// Loops until the admin code is not entered and the donation is valid
		while (adminTest && tester) {
			puts("\nEnter the amount you want to donate\n");
			fgets(inputStr, LENGTH, stdin);

			// Getting rid of the new line
			inputLength = strnlen(inputStr, LENGTH);
			if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}

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
				double ifZero = strtod(inputStr, &end); // Input string into a double
				// Checks if what was entered is 0
				if (ifZero <= 0) {
					puts("Donation has to be greater than 0");
				}
				else {
					tester = false;
				}
			}
		}
	} while (adminTest && !validateInt(inputStr));

	// Updates what the user donated
	*userDonated = strtod(inputStr, &end);
	return adminTest;
}

// Gets the donater's name
void getDonaterName(char* donaterName) {
	char userName[LENGTH]; // What was entered
	size_t inputLength = 0; // For the new line

	puts("\nEnter first and last name\n");
	fgets(userName, LENGTH, stdin);

	// Getting rid of the new line
	inputLength = strnlen(userName, LENGTH);
	if (inputLength > 0 && userName[inputLength - 1] == '\n') {
		userName[inputLength - 1] = '\0';
		inputLength--;
	}
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
		inputLength = strnlen(zipCode, LENGTH);
		if (inputLength > 0 && zipCode[inputLength - 1] == '\n') {
			zipCode[inputLength - 1] = '\0';
			inputLength--;
		}

		// If the length is not 5
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
		else if  (valid && !validateInt(zipCode)) {
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
	char input[LENGTH];
	size_t inputLength = 0;
	bool test = true;

	while (test) {
		puts("\nDo you want a receipt (y)es or (n)o?");
		fgets(input, LENGTH, stdin);
		inputLength = strnlen(input, LENGTH);

		if (inputLength > 0 && input[inputLength - 1] == '\n') {
			input[inputLength - 1] = '\0';
			inputLength--;
		}

		char* bufferPtr1 = strchr(input, 'y'); // If user entered y
		char* bufferPtr2 = strchr(input, 'Y'); // If user entered Y
		char* bufferPtr3 = strchr(input, 'n'); // If user entered n
		char* bufferPtr4 = strchr(input, 'N'); // If user entered N

		if (bufferPtr1 != NULL || bufferPtr2 != NULL || bufferPtr3 != NULL || bufferPtr4 != NULL) {
			printf("Organization: %s", orgName);
			printf("\nDonation Amount ($): %.1lf", userDonated);

			time_t now;
			time(&now);
			struct tm* local = localtime(&now);
			printf("\nTime is: %d/%d/%d %d:%d:%d", local->tm_mday, local->tm_mon+1, local->tm_year+1900, 
				local->tm_hour-12, local->tm_min, local->tm_sec);

			test = false;
		}
		else {
			puts("You did not enter a y or n");
		}

	} // test loop
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
	size_t inputLength = 0; // For the new line

	// For the email checks
	while (emailTest) {
		puts("Enter your email");
		fgets(userEmail, LENGTH, stdin);

		// Getting rid of the new line
		inputLength = strnlen(userEmail, LENGTH);
		if (inputLength > 0 && userEmail[inputLength - 1] == '\n') {
			userEmail[inputLength - 1] = '\0';
			inputLength--;
		}

		int compare = strcmp(userEmail, email); // If the two emails are the same
		// 0 if they are the same
		if (compare == 0) {
			emailTest = false;
		}
		// The emails are not the same
		else {
			emailCount++;
			// If two incorrect attempts have happened
			if (emailCount == 2) {
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
		inputLength = strnlen(userPassword, LENGTH);
		if (inputLength > 0 && userPassword[inputLength - 1] == '\n') {
			userPassword[inputLength - 1] = '\0';
			inputLength--;
		}

		int compare = strcmp(userPassword, password); // If the two passwords are the same
		// 0 if the passwords are the same
		if (compare == 0) {
			passwordTest = false;
		}
		// If the passwords were not equal
		else {
			passwordCount++;
			// If two incorrect attemps have happened
			if (passwordCount == 2) {
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