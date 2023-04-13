/*
* This code lets the user add and remove pets using a linked list, and puts the pets
* in a sorted order via alphabetical order
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Node structure
typedef struct node {
	char name[80];
	int age;
	struct node* nextNodePtr;
}Node;

void addPet(char* name, int* age); // Gets the name and age of the pet
void addToList(Node** headPtr, const char name[80], const int age); // Adds the pet to the list
bool deletePet(Node** headPtr); // If the user wants to delete a pet
bool delete(Node** headPtr, const char name[80]); // Deletes the pet from the list
void freeNodes(Node** headPtr); // Frees the remaining memory
void printList(Node* listPtr); // Prints the linked list
void toFile(Node* list); // Puts the pets on a file

int main(void) {
	Node* headNodePtr = NULL; // Linked list

	char nameInput[80]; // Name of pet
	int ageInput; // Age of pet
	char yesOrNo; // If the user wants to enter a pet
	do {
		// Gets the pet's information
		addPet(nameInput, &ageInput);
		addToList(&headNodePtr, nameInput, ageInput);

		// If the user wants to enter another pet
		puts("Do you want to add another pet (y)es or (n)o");
		yesOrNo = getchar();
		while (getchar() != '\n');

		yesOrNo = tolower(yesOrNo);

	} while (yesOrNo == 'y');
	toFile(headNodePtr);

	// If the user wants to delete pets
	bool test = true; // To get out of the loop
	do {
		test = deletePet(&headNodePtr);
	} while (test);

	// Frees the remaining memory
	printList(headNodePtr);
	puts("Remove all animals from memory before exiting program");
	freeNodes(&headNodePtr);
	printList(headNodePtr);

}

// Prompts the user if they want to enter a pet
void addPet(char* name, int* age) {
	char nameInput[80]; // Name input
	char ageInput[80]; // Age input
	size_t inputLength = 0; // For the new line
	char* end; // For strtod

	puts("What is the pet's name?");
	fgets(nameInput, 80, stdin);

	// Getting rid of the new line
	inputLength = strnlen(nameInput, 80);
	if (inputLength > 0 && nameInput[inputLength - 1] == '\n') {
		nameInput[inputLength - 1] = '\0';
		inputLength--;
	}

	puts("What is the pet's age?");
	fgets(ageInput, 80, stdin);

	// Getting rid of the new line
	inputLength = strnlen(ageInput, 80);
	if (inputLength > 0 && ageInput[inputLength - 1] == '\n') {
		ageInput[inputLength - 1] = '\0';
		inputLength--;
	}
	strncpy(name, nameInput, 80);
	*age = (int)strtod(ageInput, &end);

}

// Adds the pet to the list in alphabetical order
void addToList(Node** headPtr, const char name[80], const int age) {

	Node* newPtr = malloc(sizeof(Node)); // create node

	if (newPtr != NULL) { // is space available
		strncpy(newPtr->name, name, 80);
		newPtr->age = age; // place value in node
		newPtr->nextNodePtr = NULL; // node does not link to another node

		Node* previousPtr = NULL; // Connecting the nodes
		Node* currentPtr = *headPtr; // Connecting the nodes

		int compare = 0;
		if (currentPtr != NULL) {
			compare = _strcmpi(name, currentPtr->name);
		}
		// loop to find the correct location in the list  
		while (currentPtr != NULL && compare > 0) {
			previousPtr = currentPtr; // walk to ...               
			currentPtr = currentPtr->nextNodePtr; // ... next node 
			if (currentPtr->name != NULL) {
				compare = _strcmpi(name, currentPtr->name);
			}
		}

		// insert new node at beginning of list
		if (previousPtr == NULL) {
			newPtr->nextNodePtr = *headPtr;
			*headPtr = newPtr;
		}
		else { // insert new node between previousPtr and currentPtr
			previousPtr->nextNodePtr = newPtr;
			newPtr->nextNodePtr = currentPtr;
		}
	}
	else {
		puts("No memory available");
	}

}

// If the user wants to delete a pet
bool deletePet(Node** headPtr) {
	char yesOrNo; // If user wants to delete a pet
	bool test = true; // Exit the loop
	size_t inputLength = 0; // For the new line
	char inputDog[80]; // User input
	bool returnValue = true; // To exit the loop in main
	do {
		printList(*headPtr);
		
		// If the user wants to delete a pet
		puts("Do you want to delete a pet? (y)es or (n)o?");
		yesOrNo = getchar();
		while (getchar() != '\n');

		yesOrNo = tolower(yesOrNo);
		

		// If yes
		if (yesOrNo == 'y') {
			puts("Enter pet's name to delete");
			fgets(inputDog, 80, stdin);
			// Getting rid of the new line
			inputLength = strnlen(inputDog, 80);
			if (inputLength > 0 && inputDog[inputLength - 1] == '\n') {
				inputDog[inputLength - 1] = '\0';
				inputLength--;
			}
			returnValue = delete(headPtr, inputDog);
		}
		// If no
		else {
			test = false;
			returnValue = false;
		}
		// To exit the loop here
		if (!returnValue) {
			test = false;
		}

	} while (test);
	return returnValue;
}

// Deletes the pet from the list
bool delete(Node** headPtr, const char name[80]) {
	bool returnValue = true; // For the loop in main
	Node* previousPtr = NULL; // Connecting the nodes
	Node* currentPtr = *headPtr; // Connecting the nodes
	 
	if (*headPtr != NULL)
	{
		int compare = strcmp((*headPtr)->name, name); // Finding the node
		// If the first node is equal
		if (compare == 0)
		{
			*headPtr = (*headPtr)->nextNodePtr;
			free(currentPtr);
			currentPtr = NULL;
		}
		else 
		{
			// Loops until it finds an equal node
			while (currentPtr != NULL && compare != 0)
			{
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextNodePtr;
				// If there are no more nodes to iterate through
				// Means value isn't on the list
				if (currentPtr->name == NULL) {
					printf("%s was not found\n", name);
				}
				// There are more nodes
				else {
					compare = strcmp(currentPtr->name, name);
				}
			}
			if (currentPtr != NULL)
			{
				previousPtr->nextNodePtr = currentPtr->nextNodePtr;
				free(currentPtr);
				currentPtr = NULL;
			}
		}
	}
	else 
	{
		puts("There aren't any nodes in the list!");
		returnValue = false;
	}
	return returnValue;
}

// Frees the remaining memory on the list
void freeNodes(Node** headPtr) {
	Node* currentPtr = *headPtr; // To iterate
	Node* nextNodePtr = NULL; // To connect

	// Loops until list is empty
	while (currentPtr != NULL)
	{
		// Moves the rest of the nodes off of the head
		// Then deletes the head
		nextNodePtr = currentPtr->nextNodePtr;
		free(currentPtr);
		currentPtr = nextNodePtr;
	}

	*headPtr = NULL;
}

// Displays what is on the linked list
void printList(Node* listPtr) {
	if (listPtr != NULL)
	{
		puts("The names in alphabetical order: ");
		Node* currentPtr = listPtr; // To iterate

		// Prints until iterator is empty
		while (currentPtr != NULL)
		{
			printf("%s is %d years old\n", currentPtr->name, currentPtr->age);
			currentPtr = currentPtr->nextNodePtr;
		}
	}
	else
	{
		puts("List is empty");
	}
}

// Writes the pets to file
void toFile(Node* list) {
	FILE* cfPtr; // Openning the file
	Node* currentPtr = list; // To iterate through
	// If the file could be opened
	if ((cfPtr = fopen("C:\\CS2060Files\\file.txt", "w")) == NULL) {
		puts("File could not be opened");
	}
	else {
		// If there are pets on the list
		if (list != NULL) {
			// Iterates through, while putting the pets on the file
			while (currentPtr != NULL)
			{
				fprintf(cfPtr, "Name: %s Age: %d\n", currentPtr->name, currentPtr->age);
				currentPtr = currentPtr->nextNodePtr;
			}
		}
		else {
			puts("List is empty");
		}
	}
}