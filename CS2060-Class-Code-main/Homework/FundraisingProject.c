/*
* Code explanation
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

typedef struct org {
	char orgName[80];
	char purpose[80];
	char userName[80];
	double goalAmount[80];
	char email[80];
	char password[80];
} Organization;

#define PROCESSING_FEE 0.29
#define LENGTH 80
void getOrgName(Organization* org);
void getPurpose(Organization* org);
void getUserName(Organization* org);
void getGoalAmount(Organization* org);
bool validateInt(const char* buff);
void getEmail(Organization* org);
void getPassword(Organization* org);
void createURL(const Organization org);
void displayInformation(const Organization org, double amtRaised);
double getDonation(double* amtRaised);
void getDonaterName(char* donaterName);
void zipCode(const double processingFee);
void receipt(const Organization org, double userDonated);


int main(void) {
	Organization org;
	getOrgName(&org);
	getPurpose(&org);
	getUserName(&org);
	getGoalAmount(&org);
	getEmail(&org);
	getPassword(&org);
	//createURL(org);
	printf("Thank you %s. The URL to raise funds for %s is ", org.userName, org.orgName);

	double amtRaised = 0;
	double userDonated = 0;
	char donaterName[80];
	displayInformation(org, amtRaised);
	userDonated = getDonation(&amtRaised);
	getDonaterName(&donaterName);
	zipCode(PROCESSING_FEE);
	receipt(org, userDonated);


}

// Has the user input the name of the organization
void getOrgName(Organization* org) {
	char orgName[80];
	size_t inputLength = 0;

	puts("Enter Fundraising organization name");
	fgets(orgName, 80, stdin);
	inputLength = strnlen(orgName, 80);

	if (inputLength > 0 && orgName[inputLength - 1] == '\n') {
		orgName[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(org->orgName, orgName, 80);
}

// Has the user input the purpose of the organization
void getPurpose(Organization* org) {
	char purpose[80];
	size_t inputLength = 0;

	puts("\nEnter fundraiser purpose\n");
	fgets(purpose, 80, stdin);
	inputLength = strnlen(purpose, 80);

	if (inputLength > 0 && purpose[inputLength - 1] == '\n') {
		purpose[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(org->purpose, purpose, 80);
}

// Has the user enter their first and last name
void getUserName(Organization* org) {
	char userName[80];
	size_t inputLength = 0;

	puts("\nEnter first and last name\n");
	fgets(userName, 80, stdin);
	inputLength = strnlen(userName, 80);

	if (inputLength > 0 && userName[inputLength - 1] == '\n') {
		userName[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(org->userName, userName, 80);
}

// Has the user enter the goal amount of donations
void getGoalAmount(Organization* org) {
	char inputStr[80];
	size_t inputLength = 0;
		do {
			puts("Enter the goal amount you want to raise\n");
			fgets(inputStr, 80, stdin);

			inputLength = strnlen(inputStr, 80);

			if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}
		} while (!validateInt(inputStr));

		strncpy(org->goalAmount, inputStr, 80);
	
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
void getEmail(Organization* org) {
	char userEmail[80];

	puts("\nEnter email address");
	fgets(userEmail, 80, stdin);
	strncpy(org->email, userEmail, 80);
}

// Has the user enter their password
void getPassword(Organization* org) {
	char userPassword[80];

	puts("\nEnter password");
	fgets(userPassword, 80, stdin);
	strncpy(org->password, userPassword, 80);
}

// Creates the url based on what the user inputed
void createURL(const Organization org) {
	char orgName[80];
	strncpy(orgName, org.orgName, 80);
	char* bufferPtr;
	int numSpaces = 0;

	bufferPtr = strchr(orgName, ' ');

	while (bufferPtr != NULL) {
		numSpaces++;
		//bufferPtr++;

		bufferPtr = strchr(bufferPtr, ' ');
		bufferPtr = "-";
		bufferPtr++;
	} 
}

// Displays the user's organization information
void displayInformation(const Organization org, double amtRaised) {
	puts("\nURL");
	puts("\nMAKE A DIFFERENCE BY YOUR DONATION");
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
		printf("\nWe are %.1lf percent towards our goal of %.1lf", percentage, goalAmount);
	}
}

// Gets the user's donation, and adds it to the total amount raised
double getDonation(double* amtRaised) {
	char inputStr[80];
	size_t inputLength = 0;
	bool valid = true;
	char* end;
	do {
		do {
			puts("\nEnter the amount you want to donate\n");
			fgets(inputStr, 80, stdin);
			inputLength = strnlen(inputStr, 80);

			if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}
			int test = strcmp(inputStr, "0");
			if (test == 0) {
				puts("Equal");
				valid = false;
			}
			else {
				valid = true;
			}
		} while (!valid);

		inputLength = strnlen(inputStr, 80);

		if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
			inputStr[inputLength - 1] = '\0';
			inputLength--;
		}
	} while (!validateInt(inputStr));

	*amtRaised = *amtRaised + strtod(inputStr, &end);
	return strtod(inputStr, &end);
}

// Gets the donater's name
void getDonaterName(char* donaterName) {
	char userName[80];
	size_t inputLength = 0;

	puts("\nEnter first and last name\n");
	fgets(userName, 80, stdin);
	inputLength = strnlen(userName, 80);

	if (inputLength > 0 && userName[inputLength - 1] == '\n') {
		userName[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(donaterName, userName, 80);
}

// Asks the user for a zipcode and checks if it was valid
void zipCode(const double processingFee) {
	char zipCode[80];
	size_t inputLength = 0;
	bool valid = true;
	do {
		int count = 0;
		puts("Enter your 5 digit zipcode");
		fgets(zipCode, 80, stdin);
		inputLength = strnlen(zipCode, 80);

		if (inputLength > 0 && zipCode[inputLength - 1] == '\n') {
			zipCode[inputLength - 1] = '\0';
			inputLength--;
		}
		if (inputLength < 5 || inputLength > 5) {
			puts("Zipcode needs to be 5 numbers");
			valid = false;
			count++;
		}
		else if (valid && zipCode[0] == 48) {
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
		}
	} while (!valid);

}

// Promts the user if they want a receipt
// If yes, displays the donation information
void receipt(const Organization org, double userDonated) {
	char input[80];
	size_t inputLength = 0;
	char valid[80] = { "y,Y,n,N" };

	puts("Do you want a receipt (y)es or (n)o?");
	fgets(input, 80, stdin);
	inputLength = strnlen(input, 80);

	if (inputLength > 0 && input[inputLength - 1] == '\n') {
		input[inputLength - 1] = '\0';
		inputLength--;
	}

	char* bufferPtr;
	int numSpaces = 0;

	bufferPtr = strchr(input, 'yYnN');

	while (bufferPtr != NULL) {
		bufferPtr = strchr(bufferPtr, 'yYnN');
		bufferPtr++;
		if (bufferPtr != NULL) {
			puts("Valid");
		}
		else {
			puts("Invalid");
		}
	}

}