/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 04/04/2023
 * Description: Generates mandelbrot images (in ppm files) using 4 threads,
 *    each thread is in charge of a quadrant. Colors are randomnized. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct threadArgs {
  int id;
  int col0;
  int row0;
  int colEnd;
  int rowEnd;
  int size;
  struct ppm_pixel *palette;
  struct ppm_pixel *pixels;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
};

/**
* Thread function. Computes quadrants of the mandelbrot images using given data. 
* @param inputData all data required to compute. See struct above for more info. 
*/
void *computeImage(void* inputData) {
  // cast input to struct
  struct threadArgs * myArgs = (struct threadArgs *) inputData;

  // extract variables
  int threadId = myArgs->id;
  int col0 = myArgs->col0;
  int row0 = myArgs->row0;
  int colEnd = myArgs->colEnd;
  int rowEnd = myArgs->rowEnd;
  int size = myArgs->size;
  struct ppm_pixel *plt = myArgs->palette;
  struct ppm_pixel *pxl = myArgs->pixels;
  float xmin = myArgs->xmin;
  float xmax = myArgs->xmax;
  float ymin = myArgs->ymin;
  float ymax = myArgs->ymax;
  int maxIterations = myArgs->maxIterations;

  printf("Thread %d sub-image block: cols (%d, %d) to rows (%d, %d)\n", threadId, 
          col0, colEnd, row0, rowEnd);

  float xfrac, yfrac, x0, y0, x, y, xtemp;
  int iter;
  float imageSize = size;

  // compute imgae
  for (int i = row0; i < rowEnd; i++) { // i = rows
    for (int j = col0; j < colEnd; j++) { // j = cols
      xfrac = i / imageSize;
      yfrac = j / imageSize;
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);

      x = 0;
      y = 0;
      iter = 0;

      while (iter < maxIterations && (x * x + y * y) < 2 * 2) {
        xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iter++;
      }
      // put colors inside each pixel
      if (iter < maxIterations) {
        pxl[j * size + i] = plt[iter];
      }
      else {
        pxl[j * size + i].red = 0;
        pxl[j * size + i].green = 0;
        pxl[j * size + i].blue = 0;
      }
    }
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes/threads = %d\n", numProcesses);
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

  // generate pallet
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // set up threads and args
  pthread_t *threads = malloc(sizeof(pthread_t) * numProcesses);
  struct threadArgs *tArgs = malloc(sizeof(struct threadArgs) * numProcesses);

  // fill in thread args
  for (int i = 0; i < numProcesses; i++) {
    //calculate starting & ending locations
    if (i == 0) {
      // thread 1
      tArgs[i].col0 = 0;
      tArgs[i].row0 = 0;
      tArgs[i].colEnd = tArgs[i].col0 + size/2;
      tArgs[i].rowEnd = tArgs[i].row0 + size/2;
    }
    else if (i == 1) {
      // thread 2
      tArgs[i].col0 = size/2;
      tArgs[i].row0 = 0;
      tArgs[i].colEnd = tArgs[i].col0 + size/2;
      tArgs[i].rowEnd = tArgs[i].row0 + size/2;
    }
    else if (i == 2) {
      // thread 3
      tArgs[i].col0 = 0;
      tArgs[i].row0 = size/2;
      tArgs[i].colEnd = tArgs[i].col0 + size/2;
      tArgs[i].rowEnd = tArgs[i].row0 + size/2;
    }
    else {
      // thread 4
      tArgs[i].col0 = size/2;
      tArgs[i].row0 = size/2;
      tArgs[i].colEnd = tArgs[i].col0 + size/2;
      tArgs[i].rowEnd = tArgs[i].row0 + size/2;
    }
    // put other data into the args array
    tArgs[i].id = i + 1; // thread number starts from 1
    tArgs[i].palette = palette;
    tArgs[i].pixels = pixels;
    tArgs[i].size = size;
    tArgs[i].xmin = xmin;
    tArgs[i].xmax = xmax;
    tArgs[i].ymin = ymin;
    tArgs[i].ymax = ymax;
    tArgs[i].maxIterations = maxIterations;
  }

  // create threads
  for (int i = 0; i < numProcesses; i++) {
    pthread_create(&threads[i], NULL, computeImage, &tArgs[i]);
  }

  // wait for threads to join
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
    printf("Thread %d finished\n", (i + 1));
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
  free(threads);
  free(tArgs);
}
