#include "colourizer.h"
#include <stdio.h>
#include <stdlib.h>

void chunk_init(chunk chunk) {
  for (chunk.len = 0; chunk.match[chunk.len]; chunk.len++)
    ;
  chunk.kmptable = (int *)malloc(sizeof(int) * chunk.len);

  int j = 0;
  for (int i = 1; i < chunk.len; i++) {
    if (chunk.match[i] == chunk.match[j]) {
      chunk.kmptable[i] = j + 1;
      j++;
    } else {
      j = 0;
    }
    // printf("%c%d ", chunk.match[i], chunk.kmptable[i]);
  }
  // printf("\n");
}

void colourize(const char *str, chunk begin, chunk *chunks, int chunk_count) {
  /*
    index of currently active chunk
    chunk on active index will be ignored
    chunks of same colour should also be ignored
  */
  int active_chunk = -1;

  if (begin.colourtype == 4) {
    start4(begin.colour.colour4, NOBG);
  }

  for (int i = 0; str[i]; i++) {
    /*
      match chunks
      implement kmp (add kmp table to chunk structure)
      activate different chunk
    */
    printf("%c", str[i]);
  }

  // reset colour
  printf("\033[0m");
}
