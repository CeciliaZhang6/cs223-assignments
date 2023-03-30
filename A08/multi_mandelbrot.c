/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 03/28/2023
 * Description: Generates mandelbrot images (in ppm files) using 4 child processes 
 *    and splitting the work with shared memory. Colors are randomnized. 
 ---------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
* Computes quadrants of the mandelbrot images using given starting & ending points. 
* @param col0 starting point of the columns
* @param row0 starting point of the rows
* @param colEnd ending point of the columns
* @param rowEnd ending point of the rows
* @param size image size
* @param palette pointer to the palette (random color) in the shared memory
* @param pixels pointer to the pixels (image) in the shared memory
*/
void computeImage(int col0, int row0, int colEnd, int rowEnd, int size, 
                      struct ppm_pixel *palette, struct ppm_pixel *pixels) {
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  float xfrac, yfrac, x0, y0, x, y, xtemp;
  int iter;
  float imageSize = size;

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
        pixels[j * size + i] = palette[iter];
      }
      else {
        pixels[j * size + i].red = 0;
        pixels[j * size + i].green = 0;
        pixels[j * size + i].blue = 0;
      }
    }
  }
}

int main(int argc, char *argv[]){
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
    case 's':
      size = atoi(optarg);
      break;
    case 'l':
      xmin = atof(optarg);
      break;
    case 'r':
      xmax = atof(optarg);
      break;
    case 't':
      ymax = atof(optarg);
      break;
    case 'b':
      ymin = atof(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> "
             "-b <ymin> -t <ymax> -p <numProcesses>\n",
             argv[0]);
      break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // program timer
  double timer;
  struct timeval tStart, tEnd;
  // start timer
  gettimeofday(&tStart, NULL);
  // set seed
  srand(time(0));

  // set up shared memory for pixels
  int shmID_pxl;
  shmID_pxl = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * (size * size), 0644 | IPC_CREAT);
  if (shmID_pxl == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel *pixels = shmat(shmID_pxl, NULL, 0);
  if (pixels == (void *)-1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  // shared memory for palette
  int shmID_plt;
  shmID_plt = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * maxIterations, 0644 | IPC_CREAT);
  if (shmID_plt == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel *palette = shmat(shmID_plt, NULL, 0);
  if (pixels == (void *)-1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  // generate palette
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // starting and ending points for cols and rows
  int col0, row0, colEnd, rowEnd;
  // compute image with fork
  for (int i = 0; i < numProcesses; i++) {
    // calculate the starting & ending points for each child
    if (i == 0) {
      // child 1
      col0 = 0;
      row0 = 0;
      colEnd = col0 + size/2;
      rowEnd = row0 + size/2;
    }
    else if (i == 1) {
      // child 2
      col0 = size/2;
      row0 = 0;
      colEnd = col0 + size/2;
      rowEnd = row0 + size/2;
    }
    else if (i == 2) {
      // child 3
      col0 = 0;
      row0 = size/2;
      colEnd = col0 + size/2;
      rowEnd = row0 + size/2;
    }
    else {
      // child 4
      col0 = size/2;
      row0 = size/2;
      colEnd = col0 + size/2;
      rowEnd = row0 + size/2;
    }

    int pid = fork();
    if (pid == 0) {
      // child
      computeImage(col0, row0, colEnd, rowEnd, size, palette, pixels);
      exit(0);
    }
    else {
      // parent
      printf("Launched child process: %d\n", pid);
      printf("[%d] Sub-image block: cols (%d, %d) to rows (%d, %d)\n", pid, 
              col0, colEnd, row0, rowEnd);
    }
  }

  // wait for child processes to finish
  for (int i = 0; i < numProcesses; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }

  gettimeofday(&tEnd, NULL);

  // compute time
  timer = tEnd.tv_sec - tStart.tv_sec + (tEnd.tv_usec - tStart.tv_usec) / 1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // write output file
  char filename[1024];
  snprintf(filename, 1024, "multi-mandelbrot-%d-%lu.ppm", size, time(0));
  write_ppm(filename, pixels, size, size);
  printf("Writing file: %s\n", filename);

  // clean up memory
  if (shmdt(pixels) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmID_pxl, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  if (shmdt(palette) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmID_plt, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

}
