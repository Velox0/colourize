#include "colourizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chunk_init(chunk *chunk) {
  for (chunk->len = 0; chunk->match[chunk->len]; chunk->len++)
    ;
  chunk->kmptable = (int *)malloc(sizeof(int) * chunk->len);

  int j = 0;
  for (int i = 1; i < chunk->len; i++) {
    if (chunk->match[i] == chunk->match[j]) {
      chunk->kmptable[i] = j + 1;
      j++;
    } else {
      j = 0;
    }
    // printf("%c%d ", chunk.match[i], chunk.kmptable[i]);
  }
  // printf("\n");
}

int match(chunk chunk, const char *str, int start, int f) {
  if (f && chunk.type == FROM)
    start += strlen(chunk.match);
  int i, j = 0;
  for (i = start; str[i] && chunk.match[j]; i++) {
    if (chunk.match[j] == str[i]) {
      j++;
    } else
      j = chunk.kmptable[j];
  }
  if (chunk.match[j]) {
    return -1;
  }
  switch (chunk.type) {
  case FROM:
    return i - j;
  case AFTER:
  case RESETON:
    return i;
  }
}

void colourize(const char *str, chunk begin, chunk *chunks, int chunk_count) {
  /*
    index of currently active chunk
    chunk on active index will be ignored
    chunks of same colour should also be ignored
  */

  int active_chunk = -1, // index of chunk whose colour is currently enabled
      matches = 0;       // number of matches in string
  int *indicies,         // index of matching string
      *matching_chunk;   // corresponding chunk

  indicies = (int *)malloc(sizeof(int) * 10);
  matching_chunk = (int *)malloc(sizeof(int) * 10); // assuming 10 matches
                                                    // will increase if required

  for (int j = 0, start = 0, f = 0; j < chunk_count;) {
    int ind = match(chunks[j], str, start, f);

    if (ind == -1) {
      start = 0;
      f = 0;
      j++;
      continue;
    }

    // Increase size to accumulate more matches
    if (matches > 9 && matches % 10 == 0) {
      indicies = (int *)realloc(indicies, sizeof(int) * (matches + 10));
      matching_chunk =
          (int *)realloc(matching_chunk, sizeof(int) * (matches + 10));
    }

    indicies[matches] = ind;
    matching_chunk[matches] = j;

    matches++;
    start = ind;
    f = 1;
  }

  if (begin.colourtype == 4) {
    start4(begin.colour.colour4, NOBG);
  }

  for (int i = 0; str[i]; i++) {
    printf("%c", str[i]);
  }

  free(indicies);
  free(matching_chunk);

  // reset colour
  printf("\033[0m");
}
