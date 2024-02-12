#include "optionhandler.h"
#include "version.h"
#include <libclr/libclr.h>
#include <stdio.h>
#include <stdlib.h>

// One chunk consists of three arguments --[after|from|...] <string> <colour>
typedef struct {
  char *match;
  int colourtype;
  enum { FROM, AFTER, RESETON } type;
  union {
    colour24 colour24;
    colour8 colour8;
    colour4 colour4;
  } colour;
} chunk;

int main(int argc, char *argv[]) {

  chunk begin;
  begin.colourtype = -1;

  int chunk_count = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      chunk_count++;
      i++;
    }
  }

  chunk chunks[chunk_count];

  char buf[4096];
  int current_chunk = 0;
  for (int i = 1; i < argc; i++) {
    char ch;
    if (argv[i][0] == '-') {
      ch = getoption(argv[i]);
    } else if (i == 1) {
      char ch = whichcolour(argv[i]);
      if (ch > -1) {
        begin.colour.colour4 = getcolour4(0, ch);
        begin.colourtype = 4;
      } else
        fprintf(stderr, "Invalid colour: %s\n", argv[1]);
      continue;
    }
    switch (ch) {
    case 'a':
      chunks[current_chunk].type = AFTER;
      break;
    case 'f':
      chunks[current_chunk].type = FROM;
      break;
    case 'r':
      chunks[current_chunk].type = RESETON;
      break;
    case 'v':
      printf("Colourizer \033[32m" CLRVERSION "\033[0m\n");
      exit(0);
    default:
      printf("Unknow option: %s\n\n", argv[i]);
    case 'h':
      help(argv[0]);
      break;
    }

    // Next arg is matching string
    chunks[current_chunk].match = argv[i + 1];
    // Reset-on takes no colour argument
    if (chunks[current_chunk].type == RESETON) {
      current_chunk++;
      i++;
      continue;
    }
    // Next to next argument is the colour
    int clr = whichcolour(argv[i + 2]);
    if (clr > 0) {
      chunks[current_chunk].colour.colour4 = getcolour4(0, i);
    } else {
      fprintf(stderr, "Invalid colour: %s\n", argv[i + 2]);
      help(argv[0]);
      exit(0);
    }
    current_chunk++;
    i += 2;
  }

  // Testing
  // printf("chunk count: %d\n\n", chunk_count);

  // for (int i = 0; i < chunk_count; i++) {
  //   printf("Chunk %d\n", i);
  //   printf("Match: ~%s~\n", chunks[i].match);
  //   if (chunks[i].type != RESETON)
  //     printf("Colour: %d\n", chunks[i].colour.colour4);
  //   printf("\n");
  // }

  while (fgets(buf, sizeof buf, stdin) != NULL) {
    int i = 0;
    if (begin.colourtype == 4)
      start4(begin.colour.colour4, NOBG);
    while (buf[i] >= ' ') {
      printf("%c", buf[i]);
      i++;
    }
    printf("\n");
    buf[i] = '\0';
    printf("\033[0m");
  }
}
