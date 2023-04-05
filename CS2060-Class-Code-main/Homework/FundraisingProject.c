/*
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
	char orgName[LENGTH];
	char purpose[LENGTH];
	char userName[LENGTH];
	double goalAmount[LENGTH];
	char email[LENGTH];
	char password[LENGTH];
	char url[LENGTH];
} Organization;
void getOrgName(char* orgName);
void getPurpose(char* purpose);
void getUserName(char* userName);
void getGoalAmount(char* goalAmount);
bool validateInt(const char* buff);
void getEmail(char* email);
void getPassword(char* password);
void createURL(const char orgName[LENGTH], char* url, const char beginning, const char end);
void displayInformation(const Organization org, double amtRaised);
bool getDonation(double* userDonated, Organization* org);
void getDonaterName(char* donaterName);
void zipCode(const double userDonated, double* amtRaised, double* processingFee);
void receipt(const char orgName[LENGTH], const double userDonated);
bool adminValidation(char* email, char* password);
void endDisplay(const char orgName[LENGTH], const double numDonations, 
	const double amtRaised, const double processingFee);


int main(void) {
	Organization org;
	getOrgName(org.orgName);
	getPurpose(org.purpose);
	getUserName(org.userName);
	getGoalAmount(org.goalAmount);
	getEmail(org.email);
	getPassword(org.password);
	createURL(org.orgName, org.url, BEGINNING[strlen(BEGINNING)], END[strlen(END)]);
	printf("Thank you %s. The URL to raise funds for %s is %s", org.userName, org.orgName, org.url);

	double amtRaised = 0;
	double userDonated = 0;
	char donaterName[LENGTH];
	double totalProcessingFee = 0;
	unsigned int donationCounter = 0;
	displayInformation(org, amtRaised);

	
	while (getDonation(&userDonated, &org)) {
		donationCounter++;
		getDonaterName(&donaterName);
		zipCode(userDonated, &amtRaised, &totalProcessingFee);
		receipt(org.orgName, userDonated);
		displayInformation(org, amtRaised);
	}
	endDisplay(org.orgName, donationCounter, amtRaised, totalProcessingFee);
}

// Has the user input the name of the organization
void getOrgName(char* orgName) {
	char userOrgName[LENGTH];
	size_t inputLength = 0;

	puts("Enter Fundraising organization name");
	fgets(userOrgName, LENGTH, stdin);
	inputLength = strnlen(userOrgName, LENGTH);

	if (inputLength > 0 && userOrgName[inputLength - 1] == '\n') {
		userOrgName[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(orgName, userOrgName, LENGTH);
}

// Has the user input the purpose of the organization
void getPurpose(char* purpose) {
	char userPurpose[LENGTH];
	size_t inputLength = 0;

	puts("\nEnter fundraiser purpose\n");
	fgets(userPurpose, LENGTH, stdin);
	inputLength = strnlen(userPurpose, LENGTH);

	if (inputLength > 0 && userPurpose[inputLength - 1] == '\n') {
		userPurpose[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(purpose, userPurpose, LENGTH);
}

// Has the user enter their first and last name
void getUserName(char* userName) {
	char userNameInput[LENGTH];
	size_t inputLength = 0;

	puts("\nEnter first and last name\n");
	fgets(userNameInput, LENGTH, stdin);
	inputLength = strnlen(userNameInput, LENGTH);

	if (inputLength > 0 && userNameInput[inputLength - 1] == '\n') {
		userNameInput[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(userName, userNameInput, LENGTH);
}

// Has the user enter the goal amount of donations
void getGoalAmount(char* goalAmount) {
	char inputStr[LENGTH];
	size_t inputLength = 0;
		do {
			puts("Enter the goal amount you want to raise\n");
			fgets(inputStr, LENGTH, stdin);

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
	long intTest = strtol(buff, &end, 10);
	bool ifValid = false;

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
	// What was entered was a valid int
	else {
		// Converts long to int
		ifValid = true;
	}
	return ifValid;
}

// Has the user create an email for the fundraiser
void getEmail(char* email) {
	char userEmail[LENGTH];
	char emailValid[LENGTH];
	bool emailLoop = true;
	size_t inputLength = 0;
	while (emailLoop) {
		bool validLoop = true;
		puts("\nEnter email address");
		fgets(userEmail, LENGTH, stdin);
		inputLength = strnlen(userEmail, LENGTH);

		if (inputLength > 0 && userEmail[inputLength - 1] == '\n') {
			userEmail[inputLength - 1] = '\0';
			inputLength--;
		}
		while (validLoop) {
			printf("\nIs this email correct (y)es or (n)o?: %s\n", userEmail);
			fgets(emailValid, LENGTH, stdin);
			inputLength = strnlen(emailValid, LENGTH);

			if (inputLength > 0 && emailValid[inputLength - 1] == '\n') {
				emailValid[inputLength - 1] = '\0';
				inputLength--;
			}
			char* bufferPtr1;
			char* bufferPtr2;
			char* bufferPtr3;
			char* bufferPtr4;

			bufferPtr1 = strchr(emailValid, 'y');
			bufferPtr2 = strchr(emailValid, 'Y');
			bufferPtr3 = strchr(emailValid, 'n');
			bufferPtr4 = strchr(emailValid, 'N');

			if (bufferPtr1 != NULL || bufferPtr2 != NULL || bufferPtr3 != NULL || bufferPtr4 != NULL) {
				validLoop = false;
				if (bufferPtr1 != NULL || bufferPtr2 != NULL) {
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
	char userPasswordInput[LENGTH];
	size_t inputLength = 0;

	puts("\nEnter password");
	fgets(userPasswordInput, LENGTH, stdin);
	inputLength = strnlen(userPasswordInput, LENGTH);

	if (inputLength > 0 && userPasswordInput[inputLength - 1] == '\n') {
		userPasswordInput[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(password, userPasswordInput, LENGTH);
}

// Creates the url based on what the user inputed
void createURL(const char orgName[LENGTH], char* url, const char beginning, const char end) {
	char tempOrgName[LENGTH];
	strncpy(tempOrgName, orgName, LENGTH);
	char* bufferPtr;

	bufferPtr = strchr(tempOrgName, ' ');

	while (bufferPtr != NULL) {
		*bufferPtr = '-';
		bufferPtr++;
		bufferPtr = strchr(tempOrgName, ' ');
	} 
	
	char tempChar[LENGTH];
	strncpy(tempChar, "https:donate.com/[", LENGTH);
	strcat(tempChar, tempOrgName);
	strcat(tempChar, "]?form=popup#");
	int test = 0;
	while ( test < strlen(tempChar)) {
		tempChar[test] = tolower(tempChar[test]);
		test++;
	}
	strncpy(url, tempChar, LENGTH);
	
}

// Displays the user's organization information
void displayInformation(const Organization org, double amtRaised) {
	puts("\n\nMAKE A DIFFERENCE BY YOUR DONATION");
	printf("\nOrganization: %s", org.orgName);
	printf("\nPurpose: %s", org.purpose);
	printf("\nWe have currently raised $%.1lf", amtRaised);
	char* end;
	double goalAmount = strtod(org.goalAmount, &end);
	if (amtRaised >= goalAmount) {
		puts("\nWe have reached our goal but could still use donations");
	}
	else {
		double percentage = amtRaised / goalAmount;
		percentage = percentage * 100;
		printf("\nWe are %.1lf percent towards our goal of $%.1lf", percentage, goalAmount);
	}
}

// Gets the user's donation, and adds it to the total amount raised
bool getDonation(double* userDonated, Organization* org) {
	char inputStr[LENGTH];
	size_t inputLength = 0;
	bool tester = true;
	char* end;
	bool adminTest = true;
	do {
		while (adminTest && tester) {
			puts("\nEnter the amount you want to donate\n");
			fgets(inputStr, LENGTH, stdin);
			inputLength = strnlen(inputStr, LENGTH);

			if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}

			char test = inputStr[0];

			if (test == 'q' || test == 'Q') {
				if (adminValidation(org->email, org->password)) {
					adminTest = false;
				}
			}
			else {
				char* valid;
				valid = strchr(&test, '0');
				if (valid != NULL) {
					puts("Donation has to be greater than 0");
				}
				else {
					tester = false;
				}
			}
		}
		inputLength = strnlen(inputStr, LENGTH);

		if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
			inputStr[inputLength - 1] = '\0';
			inputLength--;
		}
	} while (adminTest && !validateInt(inputStr));

	*userDonated = strtod(inputStr, &end);
	return adminTest;
}

// Gets the donater's name
void getDonaterName(char* donaterName) {
	char userName[LENGTH];
	size_t inputLength = 0;

	puts("\nEnter first and last name\n");
	fgets(userName, LENGTH, stdin);
	inputLength = strnlen(userName, LENGTH);

	if (inputLength > 0 && userName[inputLength - 1] == '\n') {
		userName[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(donaterName, userName, LENGTH);
}

// Asks the user for a zipcode and checks if it was valid
void zipCode(const double userDonated, double* amtRaised, double* processingFee) {
	char zipCode[LENGTH];
	size_t inputLength = 0;
	bool valid = true;
	do {
		valid = true;
		int count = 0;
		puts("Enter your 5 digit zipcode");
		fgets(zipCode, LENGTH, stdin);
		inputLength = strnlen(zipCode, LENGTH);

		if (inputLength > 0 && zipCode[inputLength - 1] == '\n') {
			zipCode[inputLength - 1] = '\0';
			inputLength--;
		}
		if (inputLength < 5 || inputLength > 5) {
			puts("Zipcode needs to be 5 numbers");
			valid = false;
			count++;
		}
		else if (valid && zipCode[0] == '0') {
			puts("Zipcode cannot begin with the number 0");
			valid = false;
			count++;
		} 
		else if  (valid && !validateInt(zipCode)) {
			valid = false;
			count++;
		}
		else if (count == 0) {
			valid = true;
			printf("\nThere is a %.1lf percent credit card processing fee of %.1lf. ",
				PROCESSING_FEE, userDonated);
			double tempValue = userDonated;
			double tempProcessingFee = tempValue * (double)(PROCESSING_FEE / 100);
			*processingFee = *processingFee + tempProcessingFee;
			double tempUserDonated = tempValue - tempProcessingFee;
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

		char* bufferPtr1;
		char* bufferPtr2;
		char* bufferPtr3;
		char* bufferPtr4;

		bufferPtr1 = strchr(input, 'y');
		bufferPtr2 = strchr(input, 'Y');
		bufferPtr3 = strchr(input, 'n');
		bufferPtr4 = strchr(input, 'N');

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
	bool returnValue = true;
	bool emailTest = true;
	bool passwordTest = true;
	unsigned int emailCount = 0;
	unsigned int passwordCount = 0;
	char userEmail[LENGTH];
	char userPassword[LENGTH];
	size_t inputLength = 0;
	while (emailTest) {
		puts("Enter your email");
		fgets(userEmail, LENGTH, stdin);

		inputLength = strnlen(userEmail, LENGTH);
		if (inputLength > 0 && userEmail[inputLength - 1] == '\n') {
			userEmail[inputLength - 1] = '\0';
			inputLength--;
		}
		int compare = strcmp(userEmail, email);
		if (compare == 0) {
			emailTest = false;
		}
		else {
			emailCount++;
			if (emailCount == 2) {
				emailTest = false;
				returnValue = false;
			}
		}
	}
	while (returnValue && !emailTest && passwordTest) {
		puts("Enter your password");
		fgets(userPassword, LENGTH, stdin);

		inputLength = strnlen(userPassword, LENGTH);
		if (inputLength > 0 && userPassword[inputLength - 1] == '\n') {
			userPassword[inputLength - 1] = '\0';
			inputLength--;
		}
		int compare = strcmp(userPassword, password);
		if (compare == 0) {
			passwordTest = false;
		}
		else {
			passwordCount++;
			if (passwordCount == 2) {
				passwordTest = false;
				returnValue = false;
			}
		}
	}
	return returnValue;
}

void endDisplay(const char orgName[LENGTH], const double numDonations,
	const double amtRaised, const double processingFee) {
	puts("Donation Summary:");
	printf("\nOrganization Name: %s", orgName);
	printf("\nTotal Number of donations: %.1lf", numDonations);
	printf("\nTotal amount raised: $%.1lf", amtRaised);
	printf("\nTotal amount paid for credit card processing: $%.1lf", processingFee);
}