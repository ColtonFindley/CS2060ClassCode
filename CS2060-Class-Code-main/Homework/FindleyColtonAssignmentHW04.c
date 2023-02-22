// Colton Findley

#include <stdio.h>

#define STUDENTS 4
#define GRADE_CATEGORIES 5
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final " 
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, .15 };

void print1DArray(const double grades[], size_t categories);
double enterGrades(double array[][GRADE_CATEGORIES], size_t students, size_t categories);
void print2DArray(const double array[][GRADE_CATEGORIES], size_t students, size_t categories);
void finalGrade(const double array[][GRADE_CATEGORIES], size_t students, size_t categories);

int main(void) {

	printf("This program will calculate the grades for these categories %s", CATEGORIES);

	print1DArray(GRADE_CATEGORY_WEIGHT, GRADE_CATEGORIES);

	printf("\nThe correct order to enter grades for each student is: %s", CATEGORIES);

	double studentArray[STUDENTS][GRADE_CATEGORIES] = { 0 };
	studentArray[STUDENTS][GRADE_CATEGORIES] = enterGrades(studentArray, STUDENTS, GRADE_CATEGORIES);

	print2DArray(studentArray, STUDENTS, GRADE_CATEGORIES);

	finalGrade(studentArray, STUDENTS, GRADE_CATEGORIES);



}

void print1DArray(const double grades[], size_t categories) {
	puts("The category weights are");

	for (size_t i = 0; i < categories; ++i) {
		printf("\nCategory %d weight is %.2lf", i+1, grades[i]);
	}

}

double enterGrades(double array[][GRADE_CATEGORIES], size_t students, size_t categories) {
	puts("");
	puts("");
	int validateInput = 0;
	for (size_t row = 0; row < students; ++row) {
		for (size_t column = 0; column < categories; ++column) {
			printf("Enter the grade for each category for student %d, category %d: ", row+1, column+1);
			validateInput = scanf("%lf", &array[row][column]);
			while ((getchar()) != '\n');
			while (validateInput != 1 || array[row][column] < 0 || array[row][column] > 105) {
				printf("Enter the grade for each category for student %d, category %d: ", row + 1, column + 1);
				validateInput = scanf("%lf", &array[row][column]);
				while ((getchar()) != '\n');
			}

		}
		puts("");
	}

	return array[students][categories];
}

void print2DArray(const double array[][GRADE_CATEGORIES], size_t students, size_t categories) {

	puts("Grades entered for each student");
	for (size_t row = 0; row < students; ++row) {
		printf("Student %d:", row+1);
		for (size_t column = 0; column < categories; ++column) {
			printf(" %.2lf ", array[row][column]);
		}
		puts("");
	}

}

	void finalGrade(const double array[][GRADE_CATEGORIES], size_t students, size_t categories) {
	unsigned int sum = 0; 
	double average = 0;
	for (size_t row = 0; row < students; ++row) {
		sum = 0;
		printf("Student %d: ", row + 1);
		for (size_t column = 0; column < categories; ++column) {
			sum += array[row][column];
		}
		average = sum / categories;
		printf("%.1lf", average);
		puts("");
	}

}