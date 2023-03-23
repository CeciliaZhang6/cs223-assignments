#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // program timer
  double timer;
  struct timeval tStart, tEnd;
  // start timer
  gettimeofday(&tStart, NULL);
  //set seed
  srand(time(0));

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * size * size);
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations); 
  float xfrac, yfrac, x0, y0, x, y, xtemp;
  int iter;
  float imageSize = size;

  //generate palette
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // compute image
  for (int i = 0; i < size; i++) {  // i = rows
    for (int j = 0; j < size; j++) {  // j = cols
      xfrac = i / imageSize; 
      yfrac = j / imageSize; 
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);

      x = 0;
      y = 0;
      iter = 0;

      while (iter < maxIterations && (x*x + y*y) < 2*2) {
        xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iter++;
      }
      //put colors inside each pixel
      if (iter < maxIterations) {
        pixels[j * size + i] = palette[iter];
      } else {
        pixels[j * size + i].red = 0;
        pixels[j * size + i].green = 0;
        pixels[j * size + i].blue = 0;
      }
    }
  }
  gettimeofday(&tEnd, NULL);

  // compute time
  timer = tEnd.tv_sec - tStart.tv_sec + (tEnd.tv_usec - tStart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // write output file
  char filename[1024];
  snprintf(filename, 1024, "mandelbrot-%d-%lu.ppm", size, time(0)); 
  write_ppm(filename, pixels, size, size);
  printf("Writing file: %s\n", filename);

  free(pixels);
  free(palette); 
}
