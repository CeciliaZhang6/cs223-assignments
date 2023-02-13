/*----------------------------------------------
 * File Name: slow.c
 * Author: Cecilia Zhang
 * Date: 02/06/2023
 * Description: Takes a given phrase and convert it into ent-speach 
 * that contains '.' between each letter. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  //Text and length given by user
  char buffer[32];
  int pauseLen = 0;

  //prompt
  printf("Pause length: ");
  scanf("%d", &pauseLen);
  printf("Text: ");
  scanf("%s", buffer);

  char *result = NULL;
  result = malloc(sizeof(char) * ((pauseLen + 1) * strlen(buffer) + 1));
  
  int index = 0; //for result char
  for (int i = 0; i < strlen(buffer); i++) { //append each char in the word
    result[index] = buffer[i];
    for (int j = 1; j < pauseLen + 1; j++) { //add "." after each char
      result[index + j] = '.';
    }
    //update index for result char
    index += pauseLen;
    index++;
  }
  //add termination char
  result[(pauseLen + 1) * strlen(buffer)] = '\0';
  printf("%s\n", result);

  free(result);
  return 0;
}
