#include <stdio.h>
#include <stdlib.h>

int f(int n) {//recursion function
  if (n == 0) {//base case
    return 2;
  } else {
    return 2 * n + f(n - 1) - 1;
  }
}

int main(int argc, char *argv[]) {
  int n;
  n = atoi(argv[1]);//read the input
  if (n==0){
    printf("2\n");
    return 0;
  }
  if (n <= 0 || argc <2 || atoi(argv[1])<0 ) {//check if input is legal or not
    printf("There is something wrong with your input. Forget to input or input a negative number?\n");
    return 1;
  }
  printf("%d\n", f(n));
  return 0;
}
