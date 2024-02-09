#include "version.h"
#include <libclr/libclr.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("libclr: " LIBCLRVERSION "\n");
  printf("clr: " CLRVERSION "\n");
  return 0;
}