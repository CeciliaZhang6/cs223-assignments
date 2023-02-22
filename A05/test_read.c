/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/21/2023
 * Description: Tests the read_ppm() function and prints out the RGB values 
 *   of each pixel it read. Width and height are also read and printed. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  //read ppm file
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  //if file cannot open
  if (pixels == NULL) {
    exit(0);
  }

  //print out the grid of pixels
  printf("Testing file feep-raw.ppm: w = %d, h = %d\n", w, h);
  for (int i = 0; i < w; i++){
    for (int j = 0; j < h; j++){
      printf("(%u, %u, %u) ", pixels[i * w + j].red, pixels[i * w + j].green, 
      pixels[i * w + j].blue);
    }
    printf("\n");
  }
  free(pixels);
  pixels = NULL;
  return 0;
}

