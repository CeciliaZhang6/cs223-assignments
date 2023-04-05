/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 04/04/2023
 * Description: Reads in a binary file and stores data into an int array.
 *    Utilized multiple threads to search simultaneously for a given int. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

struct threadArgs {
  int startIdx;
  int endIdx;
  int* nums;
  int targetNum;
  int id;
  int found;
};

/**
* Thread function. Searches for the target number and prints out its index if exists. 
* @param inputData all data required to compute. See struct above for more info. 
*/
void *searchNum(void* inputData) {
  // cast input to struct
  struct threadArgs * myArgs = (struct threadArgs *) inputData;

  // extract variables
  int threadId = myArgs->id;
  int* nums = myArgs->nums;
  int startIndex = myArgs->startIdx;
  int endIndex = myArgs->endIdx;
  int targetNum = myArgs->targetNum;

  // search
  for (int i = startIndex; i < endIndex; i++) {
    if (nums[i] == targetNum){
      printf("Thread %d found %d at index %d\n", threadId, targetNum, i);
      myArgs->found = 1;
    }
  }
  return NULL;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  int size = 0;
  FILE* fp = fopen("data.bin", "rb");

  // read array size
  fread(&size, sizeof(int), 1, fp);
  int *nums = NULL;
  nums = malloc(sizeof(int) * size);
  fread(nums, sizeof(int), size, fp);

  // the number we will look for
  int targetNum = 0;
  printf("Enter a value to search: ");
  scanf("%d", &targetNum);

  // set up threads
  int numThreads = atoi(argv[1]);
  pthread_t *threads = malloc(sizeof(pthread_t) * numThreads);
  struct threadArgs *tArgs = malloc(sizeof(struct threadArgs) * numThreads);

  // fill in thread args
  for (int i = 0; i < numThreads; i++) {
    tArgs[i].startIdx = i * (size/numThreads);
    tArgs[i].endIdx = tArgs[i].startIdx + (size/numThreads);
    tArgs[i].nums = nums;
    tArgs[i].id = i + 1; // thread number starts from 1
    tArgs[i].targetNum = targetNum;
    tArgs[i].found = 0;
  }

  // create threads
  for (int i = 0; i < numThreads; i++) {
    pthread_create(&threads[i], NULL, searchNum, &tArgs[i]);
  }

  // wait for thread join
  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  // check if not found in all threads
  int notFound = 1;
  for (int i = 0; i < numThreads; i++) {
    if (tArgs[i].found == 1) {
      notFound = 0;
    }
  }
  if (notFound == 1) {
    printf("Not found!\n");
  }

  fclose(fp);
  free(nums);
  free(threads);
  free(tArgs);
  return 0;
}
