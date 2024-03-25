#include "colourizer.h"
#include "clusterfudge.h"
#include <libclr/colourmods.h>
#include <libclr/display.h>
#include <libclr/libclr.h>
#include <stdio.h>
#include <stdlib.h>

void chunk_init(chunk *chunk) {
  // allocate space for kmp table
  for (chunk->len = 0; chunk->match[chunk->len]; chunk->len++)
    ;
  chunk->kmptable = (int *)malloc(sizeof(int) * chunk->len);

  // generate kmp table
  for (int i = 1, j = 0; i < chunk->len;) {
    if (compare(chunk->match[i], chunk->match[j])) {
      j++;
      chunk->kmptable[i] = j;
      i++;
    } else {
      if (j != 0)
        j = chunk->kmptable[j - 1];
      else {
        chunk->kmptable[i] = 0;
        i++;
      }
    }
  }
}

// print ansi sequence associated with given chunk
void start_colour(chunk current, chunk begin, chunk before) {

  // avoid repeating same colour
  if (!is_different_colour(current, before))
    return;

  if (current.type == RESETON) {
    if (begin.colourtype == 4)
      start4(begin.colour.colour4, NOBG);
    else
      printf("\e[0m");
  } else if (current.colourtype == 4) {
    start4(current.colour.colour4, NOBG);
  } else if (current.colourtype == 24) {
    start24(current.colour.colour24);
  }
}

void colourize(const char *str, chunk begin, chunk *chunks, int chunk_count) {
  int active_chunk = -1, // index of chunk whose colour is currently enabled
      matches = 0;       // number of matches in string
  int *indicies,         // index of matching string
      *matching_chunk;   // corresponding chunk

  indicies = (int *)malloc(sizeof(int) * 10);
  matching_chunk = (int *)malloc(sizeof(int) * 10); // assuming 10 matches
                                                    // will increase if required

  for (int j = 0, start = 0, f = 0; j < chunk_count;) {
    // skip if no colour type
    if (chunks[j].colourtype == -1) {
      j++;
      continue;
    }

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
      printf("\e[0m\n");
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

  while (str[i]) {
    printf("%c", str[i++]);
  }

  free(indicies);
  free(matching_chunk);

  // reset colour
  printf("\e[0m");
}
