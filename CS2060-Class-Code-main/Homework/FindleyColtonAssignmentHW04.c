// Colton Findley
// This code simulates Canvas, and it works with 2D and 1D arrays to calculate students' grades with weights

#include <stdio.h>

#define STUDENTS 4 // Number of students
#define GRADE_CATEGORIES 5 // Number of grade weights
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final " // Categories of weights
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, 0.15 }; // ALl the grade weights

void print1DArray(const double grades[], size_t categories); // Prints the 1D array
double enterGrades(double array[][GRADE_CATEGORIES], size_t students, size_t categories); // User entering grades
void print2DArray(const double array[][GRADE_CATEGORIES], size_t students, size_t categories); // Prints the 2D array
double finalGrade(const double array[][GRADE_CATEGORIES], const double grades[], size_t students, size_t categories, double finalGrades[]); // Makes the final grade array
void finalPrint(const double finalGrades[], size_t students); // Prints the final array and total average grades

int main(void) {

	printf("This program will calculate the grades for these categories %s", CATEGORIES);
	puts("");

	print1DArray(GRADE_CATEGORY_WEIGHT, GRADE_CATEGORIES);

	printf("\n\nThe correct order to enter grades for each student is: %s", CATEGORIES);

	double studentArray[STUDENTS][GRADE_CATEGORIES] = { 0 }; // 2D array
	enterGrades(studentArray, STUDENTS, GRADE_CATEGORIES);

	print2DArray(studentArray, STUDENTS, GRADE_CATEGORIES);

	double finalGradeArray[STUDENTS] = { 0 } ; // 1D array
	finalGrade(studentArray, GRADE_CATEGORY_WEIGHT, STUDENTS, GRADE_CATEGORIES, finalGradeArray);

	finalPrint(finalGradeArray, STUDENTS);
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
	int validateInput = 0; // If scanf was valid
	
	// Iteration of 2D array
	for (size_t row = 0; row < students; ++row) {

		for (size_t column = 0; column < categories; ++column) {

			// User entering input
			printf("Enter the grade for each category for student %d, category %d: ", row+1, column+1);
			validateInput = scanf("%lf", &array[row][column]);
			while ((getchar()) != '\n'); // Clear buffer

			// If the user entered data was valid
			while (validateInput != 1 || array[row][column] < 0 || array[row][column] > 105) {
				printf("Enter the grade for each category for student %d, category %d: ", row + 1, column + 1);
				validateInput = scanf("%lf", &array[row][column]);
				while ((getchar()) != '\n'); // Clear buffer
			}

		}
		puts("");
	}

	return array[students][categories];
}

void print2DArray(const double array[][GRADE_CATEGORIES], size_t students, size_t categories) {
	puts("Grades entered for each student");

	// Iterating through the 2D array
	for (size_t row = 0; row < students; ++row) {
		printf("Student %d:", row+1);
		for (size_t column = 0; column < categories; ++column) {
			printf(" %.1lf ", array[row][column]);
		}
		puts("");
	}
	puts("");
}

double finalGrade(const double array[][GRADE_CATEGORIES], const double grades[], size_t students, size_t categories, double finalGrades[]) {
	double sum = 0; // Sum of the grades after the weight is applied
	unsigned int iteration = 0; // Iteration for adding values to the return value

	// Iterating through the 2D array
	for (size_t row = 0; row < students; ++row) {
		sum = 0;
		for (size_t column = 0; column < categories; ++column) {
			sum += array[row][column] * grades[column]; // Sum is equal to user input multiplied by the specific weight
		}
		finalGrades[iteration] = sum;
		iteration++;
	}

	return finalGrades[students];
}

void finalPrint(const double finalGrades[], size_t students) {
	
	puts("Final grades for students, respectively");
	double totalAverage = 0; // Final average of students

	// Iterating through the 1D array
	for (size_t i = 0; i < students; ++i) {
		totalAverage += finalGrades[i];
		printf("Student %d: ", i + 1);

		// Assigning letter to grade
		if (finalGrades[i] >= 90) {
			printf("%.1lf A", finalGrades[i]);
		}
		else if (finalGrades[i] < 90 && finalGrades[i] >= 80) {
			printf("%.1lf B", finalGrades[i]);
		}
		else if (finalGrades[i] < 80 && finalGrades[i] >= 70) {
			printf("%.1lf C", finalGrades[i]);
		}
		else if (finalGrades[i] < 70 && finalGrades[i] >= 60) {
			printf("%.1lf D", finalGrades[i]);
		}
		else if (finalGrades[i] < 60) {
			printf("%.1lf F", finalGrades[i]);
		}
		puts("");
	}
	// Calculating the total student average
	totalAverage = totalAverage / students;
	printf("\nClass average is %.1lf", totalAverage);

}