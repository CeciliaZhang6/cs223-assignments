/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/27/23
 * Description: Creates a bitmap based on the given unsigned long. 
 *  "@" represents 1, and space represents 0. 
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  for (int row = 0; row < 8; row++){ 
    for (int col = 0; col < 8; col++){ 
      //starting from left most bit
      unsigned long bit = img >> (63 - (row * 8 + col));
      //mask to extract left most bit
      unsigned long mask = 0x1;
      if (bit & mask) { //when the bit is 1
        printf("@ ");
      }
      else { // when the bit is 0
        printf("  ");
      }
    }
    printf("\n");
  }
  
  return 0;
}
