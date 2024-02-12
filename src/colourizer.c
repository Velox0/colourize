#include "colourizer.h"
#include <stdio.h>

void colourize(const char *str, chunk begin, chunk *chunks, int chunk_count) {
  if (begin.colourtype == 4) {
    start4(begin.colour.colour4, NOBG);
  }

  printf("%s", str);

  // reset colour
  printf("\033[0m");
}
