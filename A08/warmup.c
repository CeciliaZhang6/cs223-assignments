#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  printf("%d | A0\n", getpid());

  pid = fork();

  if (pid) {
    printf("%d | B0\n", getpid());
    pid = fork();
    if (pid){
      printf("%d | C0\n", getpid());
    }
    else {
      printf("%d | C1\n", getpid());
    }
  }
  else {
    printf("%d | B1\n", getpid());
  }
  
  printf("%d | Bye\n", getpid());
  return 0;
}
