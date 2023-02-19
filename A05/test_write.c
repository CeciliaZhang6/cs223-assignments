/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // test writing the file to test.ppm, reload it, and print the contents
  free(pixels);
  return 0;
}
