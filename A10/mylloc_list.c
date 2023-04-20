/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 04/18/2023
 * Description: Self-defined malloc and free.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0) {
    return NULL;
  }

  struct chunk *next = flist;
  struct chunk *prev = NULL;

  // look for a chunk in flist that has enough size for new buffer
  while (next != NULL){
    if (next->size >= size){
      // existing chunk available
      if (prev != NULL){
        prev->next = next->next;
      }
      else {
        flist = next->next;
      }
      // update used to the newly requested size
      next->used = size;
      return (void *)(next + 1);
    }
    else {
      // no existing chunk in flist can be reused, add new chunk
      prev = next;
      next = next->next;
    }
  }
  
  // allocate memory for the newly added
  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1){
    return NULL;
  }
  else {
    struct chunk *cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->used = size;
    return (void*)(cnk + 1);
  }
}

void free(void *memory) {
  // check if all memory freed
  if (memory != NULL) { 
    // go back to previous chunk
    struct chunk *cnk = (struct chunk *) ((struct chunk *) memory - 1);
    // add previous chunk to the begining of flist
    cnk->next = flist;
    flist = cnk;
  }
  return;
}
