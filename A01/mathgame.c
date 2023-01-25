/***************************************************
 * mathgame.c
 * Author: Cecilia Zhang 
 * Modified: Jan 22, 2023
 * Modified: Jan 23, 2023
 * Modified: Jan 24, 2023
 * Implements a simple math game that generates random addition problems.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  //welcome message
  printf("Welcome to Math Game!\n");
  //set seed
  time_t t;
  srand((unsigned) time(&t));
  //total rounds (based on user)
  int round = 0; 
  //count for correct answers
  int correct = 0; 

  //ask for number of roudns
  printf("How many rounds do you want to play?\n");
  scanf("%d", &round);
  //question loop
  for (int i = 0; i < round; i++){
    //generate random numbers between 1 and 9
    int numA = rand() % 9 + 1;
    int numB = rand() % 9 + 1;
    int answer = 0; //user's answer
    //ask questions
    printf("%d + %d = ? ", numA, numB);
    scanf("%d", &answer);
    //check correctness
    if (numA + numB == answer){
      correct ++;
      printf("Correct! :) \n");
    }
    else {
      printf("Incorrect :( \n");
    }
  }
    
  //completion message
  printf("You answered %d/%d correctly.\n", correct, round);

  return 0;
}