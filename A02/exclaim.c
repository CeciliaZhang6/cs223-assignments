/***************************************************
 * exclaim.c
 * Author: Cecilia Zhang 
 * Modified: Jan 29, 2023
 * Modified: Jan 30, 2023
 * Implements a program that loops through a given string and replaces lowercase 
 * characters to a random symbol.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {  
  //set seed
  time_t t;
  srand((unsigned) time(&t));

  //word given by user
  char buffer[32];
  //symbols that will replace lowercase chars
  char symbols[5] = {'@', '!', '#', '*'};

  //prompt
  printf("Enter a word: ");
  scanf("%s", buffer);
  //check each letter to see if it is lowercase
  for (int i = 0; i < strlen(buffer); i++) {
    if (buffer[i] >= 'a' && buffer[i] <= 'z') {
      //replace lowercase with a random symbol
      buffer[i] = symbols[rand() % strlen(symbols)];
    }
  }
  
  printf("OMG! %s! \n", buffer);
  return 0;
}
