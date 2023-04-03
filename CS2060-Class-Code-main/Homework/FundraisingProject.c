/*
* Code explanation
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

typedef struct org {
	char orgName[80];
	char purpose[80];
	char userName[80];
	double goalAmount[80];
	char email[80];
	char password[80];
} Organization;
void getOrgName(Organization* org);
void getPurpose(Organization* org);
void getUserName(Organization* org);
void getGoalAmount(Organization* org);
bool validateInt(const char* buff, int* validInt);
void displayOrg(const Organization org);
void getEmail(Organization* org);
void getPassword(Organization* org);
void createURL(const Organization org);

int main(void) {

	Organization org;
	getOrgName(&org);
	getPurpose(&org);
	getUserName(&org);
	getGoalAmount(&org);
	getEmail(&org);
	getPassword(&org);
	displayOrg(org);



}

// Has the user input the name of the organization
void getOrgName(Organization* org) {
	char orgName[80];

	puts("Enter Fundraising organization name");
	fgets(orgName, 80, stdin);
	strncpy(org->orgName, orgName, 80);
}

// Has the user input the purpose of the organization
void getPurpose(Organization* org) {
	char purpose[80];

	puts("\nEnter fundraiser purpose\n");
	fgets(purpose, 80, stdin);
	strncpy(org->purpose, purpose, 80);
}

// Has the user enter their first and last name
void getUserName(Organization* org) {
	char userName[80];

	puts("\nEnter first and last name\n");
	fgets(userName, 80, stdin);
	strncpy(org->userName, userName, 80);
}

// Has the user enter the goal amount of donations
void getGoalAmount(Organization* org) {
	char inputStr[80];
	size_t inputLength = 0;
	int validInt = 0;
		do {
			puts("Enter the goal amount you want to raise\n");
			fgets(inputStr, 80, stdin);

			inputLength = strnlen(inputStr, 80);

			if (inputLength > 0 && inputStr[inputLength - 1] == '\n') {
				inputStr[inputLength - 1] = '\0';
				inputLength--;
			}
		} while (!validateInt(inputStr, &validInt));

		strncpy(org->goalAmount, inputStr, 80);
	
}

// Validates the user's amount input
bool validateInt(const char* buff, int* validInt) {
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

// Displays the user's organization
void displayOrg(const Organization org) {
	puts("Organization: ");
	puts(org.orgName);

	puts("Name: ");
	puts(org.userName);

	puts("Purpose: ");
	puts(org.purpose);

	puts("Goal Amount: ($)");
	puts(org.goalAmount);

}

// Creates the url based on what the user inputed
/*void createURL(const Organization org) {
	char orgName[80];
	*orgName = org.orgName;
	char value[] = "-";

	for (unsigned int i = 0; i < 80; i++) {
		if (strchr(orgName, ' ') != NULL) {
			//orgName[i] = "-";
			strcpy(orgName[i], value);
		}
	}
}
*/