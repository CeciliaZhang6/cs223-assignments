/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 04/11/2023
 * Description: Generates mandelbrot images (in ppm files) using 4 threads,
 *    each thread is in charge of a quadrant. Colors are randomnized. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <sys/time.h>
#include <pthread.h> 
#include <math.h>

pthread_barrier_t barrier;
pthread_mutex_t mutex;

struct threadArgs {
  int id;
  int col0;
  int row0;
  int colEnd;
  int rowEnd;
  int size;
  struct ppm_pixel *pixels;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int *membership;
  int *count; 
  int maxCount;
};

/**
* Thread function #1. Determine points in each quadrant if belongs to 
* the mandelbrot set or not.
* @param inputData all data required to compute. See struct above for detail. 
*/
void *findMember(void* inputData) {
  // cast input to struct
  struct threadArgs * myArgs = (struct threadArgs *) inputData;

  // extract variables
  int col0 = myArgs->col0;
  int row0 = myArgs->row0;
  int colEnd = myArgs->colEnd;
  int rowEnd = myArgs->rowEnd;
  int size = myArgs->size;
  float xmin = myArgs->xmin;
  float xmax = myArgs->xmax;
  float ymin = myArgs->ymin;
  float ymax = myArgs->ymax;
  int maxIterations = myArgs->maxIterations;

  printf("Thread %d sub-image block: cols (%d, %d) to rows (%d, %d)\n", myArgs->id, 
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
      // determine membership
      if (iter < maxIterations) { // escaped - false
        myArgs->membership[j * size + i] = 0;
      }
      else { // member - true
        myArgs->membership[j * size + i] = 1;
      }
    }
  }
  return NULL;
}

/**
* Thread function #2. Tracks the number of times visited in each
* coordinate (pixel) for each escaped points. Also finds out the max count. 
* @param inputData all data required to compute. See struct above for detail. 
*/
void *findCounts(void* inputData) {
  // cast input to struct
  struct threadArgs * myArgs = (struct threadArgs *) inputData;

  // extract variables
  int col0 = myArgs->col0;
  int row0 = myArgs->row0;
  int colEnd = myArgs->colEnd;
  int rowEnd = myArgs->rowEnd;
  int size = myArgs->size;
  float xmin = myArgs->xmin;
  float xmax = myArgs->xmax;
  float ymin = myArgs->ymin;
  float ymax = myArgs->ymax;

  float xfrac, yfrac, x0, y0, x, y, xtemp, yrow, xcol;
  float imageSize = size;
  int index = 0;

  for (int i = row0; i < rowEnd; i++) { // i = rows
    for (int j = col0; j < colEnd; j++) { // j = cols
      // check membership
      if (myArgs->membership[j * size + i] == 1) continue;

      xfrac = i / imageSize;
      yfrac = j / imageSize;
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);

      x = 0;
      y = 0;

      while (x*x + y*y < 2 * 2) {
        xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;

        yrow = round(size * (y - ymin) / (ymax - ymin));
        xcol = round(size * (x - xmin)/ (xmax - xmin));
        if (yrow < 0 || yrow >= size) continue; // out of range
        if (xcol < 0 || xcol >= size) continue; // out of range9          
        
        // update count 
        index = xcol * size + yrow;
        myArgs->count[index]++;
        // update max count
        if (myArgs->count[index] > myArgs->maxCount){
          pthread_mutex_lock(&mutex);
          myArgs->maxCount = myArgs->count[index];
          pthread_mutex_unlock(&mutex);
        }
      } // end of while
    }
  }
  return NULL;
}

/**
* Thread function #3. Computes color based on the counts from function #2. 
* Stores colors into a pixel array. 
* @param inputData all data required to compute. See struct above for detail. 
*/
void *computeColors(void* inputData) {
  float gamma = 0.681;
  float factor = 1.0/gamma;

  // cast input to struct
  struct threadArgs * myArgs = (struct threadArgs *) inputData;

  // extract variables
  int col0 = myArgs->col0;
  int row0 = myArgs->row0;
  int colEnd = myArgs->colEnd;
  int rowEnd = myArgs->rowEnd;
  int size = myArgs->size;

  float imageSize = size;
  for (int i = row0; i < rowEnd; i++) { // i = rows
    for (int j = col0; j < colEnd; j++) { // j = cols
      float value = 0;
      if (myArgs->count[j * size + i] > 0) {
        value = log(myArgs->count[j * size + i]) / log(myArgs->maxCount);
        value = pow(value, factor);
      }
      // save color
      myArgs->pixels[i * size + j].red = value * 255;
      myArgs->pixels[i * size + j].green = value * 255;
      myArgs->pixels[i * size + j].blue = value * 255;
    }
  }
  return NULL;
}

/**
* Thread function driver. Performs all steps.  
* @param inputData all data required to compute. See struct above for detail. 
*/
void *start(void* inputData) {
  struct threadArgs * myArgs = (struct threadArgs *) inputData;
  
  // perform step 1
  findMember(myArgs);
  // perform step 2
  findCounts(myArgs);
  
  // use a thread barrier to wait for all threads to finish steps 1 and 2
  pthread_barrier_wait(&barrier);
  // perform step 3
  computeColors(myArgs);
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 800;
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // program timer
  double timer;
  struct timeval tStart, tEnd;
  // start timer
  gettimeofday(&tStart, NULL);

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * size * size);
  int* membership = malloc(sizeof(int) * size * size);
  int* count = malloc(sizeof(int) * size * size);
  // initialize count
  for (int i = 0; i < (size * size); i++){
    count[i] = 0;
  }

  // set up threads, args, mutex, and barrier
  pthread_t *threads = malloc(sizeof(pthread_t) * numProcesses);
  struct threadArgs *tArgs = malloc(sizeof(struct threadArgs) * numProcesses);
  pthread_barrier_init(&barrier, NULL, numProcesses);
  pthread_mutex_init(&mutex, NULL);

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
    tArgs[i].id = i + 1;
    tArgs[i].pixels = pixels;
    tArgs[i].size = size;
    tArgs[i].xmin = xmin;
    tArgs[i].xmax = xmax;
    tArgs[i].ymin = ymin;
    tArgs[i].ymax = ymax;
    tArgs[i].maxIterations = maxIterations;
    tArgs[i].membership = membership;
    tArgs[i].count = count;
    tArgs[i].maxCount = 0;
  }

  // create threads
  for (int i = 0; i < numProcesses; i++) {
    pthread_create(&threads[i], NULL, start, &tArgs[i]);
  }

  // wait for threads to join
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
    printf("Thread %d finished\n", (i + 1));
  }

  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&mutex);

  gettimeofday(&tEnd, NULL);

  // compute time
  timer = tEnd.tv_sec - tStart.tv_sec + (tEnd.tv_usec - tStart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);

  // write output file
  char filename[1024];
  snprintf(filename, 1024, "buddhabrot-%d-%lu.ppm", size, time(0)); 
  write_ppm(filename, pixels, size, size);
  printf("Writing file: %s\n", filename);

  free(pixels);
  free(threads);
  free(count);
  free(membership);
  free(tArgs);
}
