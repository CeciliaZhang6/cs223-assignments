/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 03/26/2023
 * Description: Reads in a binary file and stores data into an int array.
 *    Utilized parent & child processes to search for a given int. 
 ---------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
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

  pid_t pid = 0;
  pid = fork();

  if (pid > 0) {
    int found = 0;
    // parent - check first half
    for (int i = 0; i < (size/2); i++) {
      if (nums[i] == targetNum){
        printf("Parent found %d at index %d \n", targetNum, i);
        found = 1;
      }
    }
    //not found
    if (found == 0) {
      printf("Not found! (from parent)\n");
    }
  }
  else {
    int found = 0;
    //child - check second half
    for (int i = size/2; i < size; i++) {
      if (nums[i] == targetNum){
        printf("Child found %d at index %d \n", targetNum, i);
        found = 1;
      }
    }
    //not found
    if (found == 0) {
      printf("Not found! (from child)\n");
    }
  }

  fclose(fp);
  free(nums);
  return 0;
}
