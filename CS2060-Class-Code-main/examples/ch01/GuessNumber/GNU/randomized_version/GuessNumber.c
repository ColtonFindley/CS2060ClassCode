/*
* The program randomly generates numbers   
* between 1 and 1000 for the user to guess.
*/


// Header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Minimum constant
#define MIN_NUMBER 1

// Maximum constant
const int MAX_NUMBER = 100;

// Adds a diagnostic
void guessGame(void); 
bool isCorrect(int, int); 


int main(void)
{
    // Seeds the random number generator
  // srand(time(0));

   // Adds a diagnostic
   guessGame(); 

   // Return value
   return 0;
}

// Method
void guessGame(void)
{
    // Variables
   int answer = 0; 
   int guess = 0; 
   int response = 0; 

   do {

       // Setting the correct answer
      answer = MIN_NUMBER + rand() % MAX_NUMBER;


      // Prints opening statement
      printf("Hello, I have a number between %d and %d .\n" 
          "Can you guess my number?\n"
           "Please type your first guess.", MIN_NUMBER, MAX_NUMBER);

      // Loop for user input
      do
      {
          // Gets user input
          scanf("%d", &guess);
      } while (!isCorrect(guess, answer));
         
      // When the answer is correct
      puts("\nExcellent! You guessed the number!\n"
         "Would you like to play again?");

      printf("%s", "Please type ( 1=yes, 2=no ) ");
      scanf("%d", &response);

      puts("");
   } while (response == 1);
} 

// Checking if the aswer is correct
bool isCorrect(int g, int a)
{
    // Initial boolean value
    bool found = false;

    // Testing if guess is equal to answer
    if (g == a){
        found = true;
    }else  if (g < a)
      printf( "%s", "Too low. Try again.\n? " );
   else
      printf( "%s", "Too high. Try again.\n? " );

    // Return type
   return found;
} 

