/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/21/2023
 * Description: Tests the write_ppm() function and prints out the RGB values 
 *   of each pixel it wrote. Width and height are also written and printed. 
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  //if file cannot open
  if (pixels == NULL) {
    exit(0);
  }

  struct ppm_pixel* outPixels = NULL;
  //write file to test.ppm
  write_ppm("feep-raw.ppm", outPixels, w, h);
  outPixels = read_ppm("feep-raw.ppm", &w, &h);

  //print out the grid of pixels
  printf("Testing file test.ppm: w = %d, h = %d\n", w, h);
  for (int i = 0; i < w; i++){
    for (int j = 0; j < h; j++){
      printf("(%u, %u, %u) ", outPixels[i * w + j].red, outPixels[i * w + j].green, 
      outPixels[i * w + j].blue);
    }
    printf("\n");
  }
  free(pixels);
  pixels = NULL;
  free(outPixels);
  outPixels = NULL;
  return 0;
}
