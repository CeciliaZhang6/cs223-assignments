/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/21/2023
 * Description: A function that reads a given ppm file, stores width, height, 
 *   and binary data of each pixel's RGB value (in an array of pixel struct). 
 *   It updates the values of width and height, and returns a pointer to the 
 *   pixel array it read in.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

/**
* See description in file header. 
* @param filename the name of input ppm file. 
* @param w int pointer to width
* @param h int pointer to height
* @return 1D array of pixels it read in. NULL if file can not be read. 
*/
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *infile;
  infile = fopen(filename, "rb");
  if (infile == NULL) {
    printf("Error! Can't open file: %s\n", filename);
    return NULL;
  }
  char buffer[1024];
  int width = 0;
  int height = 0;
  //skip title lines
  fgets(buffer, 1024, infile);
  fgets(buffer, 1024, infile);
  //read and store the width & height
  fscanf(infile, "%d%*c", &width);
  fscanf(infile, "%d%*c", &height);
  //skip line with "255"
  fgets(buffer, 1024, infile);

  struct ppm_pixel* pxl = malloc(sizeof(struct ppm_pixel) * width * height);
  fread(pxl, sizeof(struct ppm_pixel), (width * height), infile);

  fclose(infile);
  //return
  *w = width;
  *h = height;
  return pxl;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
