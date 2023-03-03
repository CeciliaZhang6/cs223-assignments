/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/21/2023
 * Description: A function that writes title lines, width, height, max color value, 
 *   and all binary data (from a given pixel array) into a given file. 
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* See description in file header. 
* @param filename the name of output file it will write to. 
* @param pixels pointer to a 1D array of pixel to save (detination).
* @param w width.
* @param h height.
* No return.
*/
void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
  FILE *outfile = fopen(filename, "wb");
  if (outfile == NULL) {
    printf("File not opened. \n");
    exit(0);
  }

  //write title lines
  fprintf(outfile, "%s\n", "P6");
  fprintf(outfile, "%s\n", "# Created by GIMP version 2.10.24 PNM plug-in");

  //write width and height
  fprintf(outfile, "%d %d\n", w, h);

  //write max color value
  fprintf(outfile, "%d\n", 255);

  //write bianry data
  fwrite(pixels, sizeof(struct ppm_pixel), (w*h), outfile);

  fclose(outfile);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
