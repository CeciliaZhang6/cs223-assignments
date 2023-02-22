/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/19/2023
 * Description: A program that generates crossword grid based on the 
 *    two words given at runtime. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* Prints out the char grid with a nice looking.
* @param grid the pointer to the grid that will be printed.
* @param col total number of columns.
* @param row total number of row.
* No return. 
*/
void printGrid(char* grid, int col, int row){
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++){
      printf("%c ", grid[i * col + j]);
    }
    printf("\n");
  }
}

/**
* Finds out the first common character in two given words. 
* @param wordA a char pointer pointing at a word that will be checked. 
* @param wordB a char pointer pointing at the other word that will be checked. 
* @return the first char in common. If no common char, returns NULL.
*/
char* findComm(char* wordA, char* wordB){
  for (int i = 0; i < strlen(wordA); i ++) { 
    for (int j = 0; j < strlen(wordB); j++){
      if (wordA[i] == wordB[j]) {
        return &wordA[i];
      }
    }
  }
  return NULL;
}

/**
* Converts an 2-D index to 1-D index. 
* @param x x-cordinate of the original 2D coordinate 
* @param y y-cordinate of the original 2D coordinate 
* @return int of the converted 1D index. 
*/
int convertIdx(int xCor, int yCor, int col){
  return xCor * col + yCor;
}

int main(int argc, char* argv[]) {
  //check runtime argument
  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }
  //store grid size and allocate
  int row = strlen(argv[1]);
  int col = strlen(argv[2]);
  char* grid = malloc(sizeof(char) * row * col);

  char* commonChar = findComm(argv[1], argv[2]);
  if (commonChar == NULL) {
    printf("No common letter!\n");
    exit(0);
  }

  //x-coordinate of the common char
  int comRow = 0;
  //y-coordinate of the common char
  int comCol = 0;
  //find coordinate
  for (int i = 0; i < strlen(argv[1]); i ++) {
    if (argv[1][i] == *commonChar) {
      comRow = i;
      break;
    }
  }
  for (int i = 0; i < strlen(argv[2]); i++){
    if (argv[2][i] == *commonChar) {
      comCol = i;
      break;
    }
  }

  //create grid with dots only
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++){
      grid[i * col + j] = '.';
    }
  }
  //place words into the grid
  for(int i = 0; i < strlen(argv[2]); i++){
    int index = convertIdx(comRow, i, col);
    grid[index] = argv[2][i];
  }
  for(int i = 0; i < strlen(argv[1]); i++){
    int index = convertIdx(i, comCol, col);
    grid[index] = argv[1][i];
  }
  printGrid(grid, col, row);
  
  free(grid);
  grid = NULL;
  return 0;
}
