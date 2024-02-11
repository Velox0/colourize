#include "optionhandler.h"
#include "version.h"
#include <libclr/colourmods.h>
#include <libclr/libclr.h>
#include <stdio.h>

typedef struct {
  char *match;
  int colourtype;
} chunk;

typedef union {
  colour24 colour24;
  colour8 colour8;
  colour4 colour4;
} colour;

int main(int argc, char *argv[]) {

  chunk begin;
  colour normal;

  char str[4096];

  for (int i = 1; i < argc; i++) {
    char ch = whichcolour(argv[i]);
    if (ch > -1)
      normal.colour4 = getcolour4(0, ch);
  }

  while (fgets(str, sizeof str, stdin) != NULL) {
    int i = 0;
    start4(normal.colour4, NOBG);
    while (str[i] >= ' ') {
      printf("%c", str[i]);
      i++;
    }
    printf("\n");
    str[i] = '\0';
    printf("\033[0m");
  }
}
