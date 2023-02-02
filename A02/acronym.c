/***************************************************
 * acronym.c
 * Author: Cecilia Zhang 
 * Modified: Jan 30, 2023
 * Implements a program that produces an acronym based on given phrase.
 */

#include <stdio.h>
#include <string.h>

int main() {
  //phrase given by user and resulting acronym
  char phrase[1024], acronym[1024];
  int index = 0; //index for the resulting acronym

  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);
  //check if each character is uppercase
  for (int i = 0; i < strlen(phrase); i++) {
    if (phrase[i] >= 'A' && phrase[i] <= 'Z') {
      //place the uppercase into acronym string
      acronym[index] = phrase[i];
      index++;
    }
  }
  acronym[index] = '\0';
  
  printf("Your acronym is %s \n", acronym);
  return 0;
}
