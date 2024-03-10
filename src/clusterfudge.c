#include "colourizer.h"
#include <stdbool.h>
#include <stdlib.h>

int compare(char a, char b){
  if(a == b)
    return 1;
  if(a <= 'Z' && b <= 'Z')
    return 0;
  if(a >= 'a' && b >= 'a')
    return 0;
  if(a <= 'Z' && b >= 'a')
    return (a-'A')-(b-'a') == 0;
  if(b <= 'Z' && a >= 'a')
    return (b-'A')-(a-'a') == 0;
  return 0;
}

int match(chunk chunk, const char *str, int start, int f) {
  if (f && chunk.type == FROM)
    start += chunk.len;
  int i, j = 0;
  for (i = start; str[i] != 0 && j < chunk.len;) {
    if (compare(chunk.match[j], str[i])) {
      j++;
      i++;
    } else if (j > 0) {
      j = chunk.kmptable[j - 1];
    } else {
      i++;
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
