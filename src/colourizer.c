#include "colourizer.h"
#include <libclr/colourmods.h>
#include <libclr/display.h>
#include <libclr/libclr.h>
#include <stdio.h>
#include <stdlib.h>

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
  }
}

int match(chunk chunk, const char *str, int start, int f) {
  if (f && chunk.type == FROM)
    start += chunk.len;
  int i, j = 0;
  for (i = start; str[i] != 0 && j < chunk.len; i++) {
    if (chunk.match[j] == str[i]) {
      j++;
    } else if (j > 0) {
      if (chunk.kmptable[j - 1] == j - 1 &&
          chunk.match[j - 1] == chunk.match[0])
        j = j;
      else
        j = chunk.kmptable[j];
    } else {
      j = chunk.kmptable[j];
    }
  }
  if (j != chunk.len) {
    return -1;
  }
  switch (chunk.type) {
  case FROM:
    return i - chunk.len;
  case AFTER:
  case RESETON:
    return i;
  }
}

void sort(int *index, int *chunk_index, int matches) {
  int i, j;
  for (i = 0; i < matches; i++) {
    int min = i;
    for (j = i + 1; j < matches; j++) {
      if (index[min] > index[j])
        min = j;
    }

    // swap indicis and chunk_index
    int temp = index[min];
    index[min] = index[i];
    index[i] = temp;

    temp = chunk_index[min];
    chunk_index[min] = chunk_index[i];
    chunk_index[i] = temp;
  }

  // for (int i = 0; i < matches; i++) {
  //   printf("%d\n", index[i]);
  // }
}

bool is_different_colour(chunk A, chunk B) {
  if (A.colourtype == B.colourtype) {
    if (A.colourtype == 4) {
      if (A.colour.colour4 == B.colour.colour4)
        return 0;
    } else if (difference24(A.colour.colour24, B.colour.colour24) == 0) {
      return 0;
    }
  }
  return 1;
}

void start_colour(chunk current, chunk begin, chunk before) {
  if (!is_different_colour(current, before))
    return;

  if (current.type == RESETON) {
    if (begin.colourtype == 4)
      start4(begin.colour.colour4, NOBG);
    else
      printf("\033[0m");
  } else if (current.colourtype == 4) {
    start4(current.colour.colour4, NOBG);
  } else if (current.colourtype == 24) {
    start24(current.colour.colour24);
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

  sort(indicies, matching_chunk, matches);

  if (begin.colourtype == 4) {
    start4(begin.colour.colour4, NOBG);
  } else if (begin.colourtype == 24) {
    start24(begin.colour.colour24);
  }

  int current_match = 0;
  int i;
  for (i = 0; str[i] && current_match < matches; i++) {
    if (str[i] == '\n') {
      printf("\033[0m\n");
      continue;
    }

    if (i == indicies[current_match]) {
      if (current_match == 0)
        start_colour(chunks[matching_chunk[current_match]], begin, begin);
      else
        start_colour(chunks[matching_chunk[current_match]], begin,
                     chunks[matching_chunk[current_match - 1]]);
      current_match++;
    }

    printf("%c", str[i]);
  }

  free(indicies);
  free(matching_chunk);

  while (str[i]) {
    printf("%c", str[i++]);
  }

  // reset colour
  printf("\033[0m");
}
