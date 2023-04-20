/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 04/18/2023
 * Description: Reports memory stats when running self-defined 
 *              malloc and free.
 ---------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  // number of chunks in free list
  int freeBlock = 0;
  // number of chunks in buffer
  int usedBlock = 0;
  // total size allocated in buffer
  int bufferSize = 0;
  // total sized actually used in buffer
  int usedSize = 0;
  // total size used in free list
  int freeSize = 0;
  // percentage of allocated but not used memory in buffer
  float underUtil = 0;

  // compute stats for free list
  while (freelist != NULL) {
    freeBlock++;
    freeSize += freelist->size;
    // update pointer to next free block
    freelist = freelist->next;
  }

  // compute stats for buffer
  for (int i = 0; i < len; i++) {
    if (buffer[i] != NULL){
      usedBlock++;
      struct chunk *header = (struct chunk *) ((struct chunk *) buffer[i] - 1);
      bufferSize += header->size; 
      usedSize += header->used;
    }
  }

  // calculate underutilzed memory percentage
  underUtil = 1.0 - ((float) usedSize / bufferSize);

  // report status
  printf("Total blocks: %d | Free blocks: %d | Used blocks: %d\n", freeBlock + usedBlock, 
        freeBlock, usedBlock);
  printf("Total memory allocated: %d | Free memory: %d | Used memory: %d\n", bufferSize + freeSize, 
        freeSize, bufferSize);
  printf("Underutilized memory: %.2f\n", underUtil);
}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
