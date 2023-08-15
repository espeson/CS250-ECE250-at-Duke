#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int i=0;// initialization
  int count = 0;
  if (argc == 1 || atoi(argv[1]) < 0) {// check if the input is negaive or not input
    printf("Invalid input! Forget to input or input a negative number?\n");
    return 1;
  }
  int n = atoi(argv[1]);// read the input and convert it into int
  for (i = 5; count < n; i++) { // begin to find the output numbers
    if (i % 5 == 0 || i % 6 == 0) {
      printf("%d\n", i);
      count++;
    }
  }
  return 0;
}
