// GE06 Work

#include <stdio.h>

void setUpOrganization(struct org1 *aOrgPtr);

int main(void) {
	typedef struct org1 {
		char organName[80];
		char name[80];
		char purpose[80];
		double goalAmount[80];
	} Organization;
	struct org1 aOrg = { "", "", "", 2 };
	setUpOrganization(&aOrg);
	int test = aOrg.goalAmount;

}

void setUpOrganization(struct org1* aOrgPtr) {

	*aOrgPtr->goalAmount = 2;


}