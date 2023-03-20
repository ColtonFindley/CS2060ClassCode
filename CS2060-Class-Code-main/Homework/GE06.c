// GE06 Work

#include <stdio.h>
#include <string.h>


typedef struct org1 {
	char organName[80];
	char name[80];
	char purpose[80];
	double goalAmount[80];
} Organization;
void setUpOrganization(Organization* aOrg);
void displayOrganization(const Organization aOrgPtr);

int main(void) {
	
	Organization aOrg;
	setUpOrganization(&aOrg);
	displayOrganization(aOrg);

}

void setUpOrganization(Organization* aOrgPtr) {
	char str[80];

	puts("Enter organization name");
	fgets(str, 80, stdin);
	strncpy(aOrgPtr->organName, str, 80);
	puts("Enter name");
	fgets(str, 80, stdin);
	strncpy(aOrgPtr->name, str, 80);
	puts("Enter purpose");
	fgets(str, 80, stdin);
	strncpy(aOrgPtr->purpose, str, 80);
	puts("Enter goal amount");
	fgets(str, 80, stdin);
	strncpy(aOrgPtr->goalAmount, str, 80);

}

void displayOrganization(const Organization aOrg) {

	puts("Organization: ");
	puts(aOrg.organName);
	
	puts("Name: ");
	puts(aOrg.name);

	puts("Purpose: ");
	puts(aOrg.purpose);

	puts("Goal Amount: ($)");
	puts(aOrg.goalAmount);


}