#include "colourizer.h"
#include "optionhandler.h"
#include "version.h"
#include <libclr/colourmods.h>
#include <libclr/libclr.h>
#include <stdio.h>
#include <stdlib.h>

#define HELP help(argv[0]);

int main(int argc, char *argv[]) {

  chunk begin; // defined in "colourizer.h"
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
  for (int arg_index = 1; arg_index < argc; arg_index++) {
    char opt;
    if (argv[arg_index][0] == '-') {
      opt = getoption(argv[arg_index]);
    } else if (arg_index == 1) {
      char ch = whichcolour(argv[arg_index]);
      if (ch > -1) {
        begin.colour.colour4 = getcolour4(0, ch);
        begin.colourtype = 4;
      } else {
        newcolour24(begin.colour.colour24);
        resetbg24(begin.colour.colour24);
        hexto24(begin.colour.colour24, NULL, argv[1]);
        begin.colourtype = 24;
      }
      continue;
    } else {
      fprintf(stderr, "Invalid parameter: %s\n\n", argv[arg_index]);
      HELP;
    }

    int ctype = 4;

    switch (opt) {
    case 'A':
      ctype = 24;
    case 'a':
      chunks[current_chunk].type = AFTER;
      break;
    case 'F':
      ctype = 24;
    case 'f':
      chunks[current_chunk].type = FROM;
      break;
    case 'r':
      chunks[current_chunk].type = RESETON;
      break;
    case 'v':
      printf("Colourizer \033[31m" CLRVERSION "\033[0m\n");
      exit(0);
    default:
      fprintf(stderr, "Unknow option: %s\n\n", argv[arg_index]);
    case 'h':
      HELP;
      break;
    }
    // Next arg is matching string
    chunks[current_chunk].match = argv[arg_index + 1];

    chunk_init(&chunks[current_chunk]);

    // Reset-on takes no colour argument
    if (chunks[current_chunk].type == RESETON) {
      current_chunk++;
      arg_index++;
      continue;
    }

    // Next to next argument is the colour
    char *colourarg = argv[arg_index + 2];
    int clr = whichcolour(colourarg);

    if (clr > -1 && ctype == 4) {
      chunks[current_chunk].colourtype = 4;
      chunks[current_chunk].colour.colour4 = getcolour4(0, clr);
    }

    else if (colourarg[0] == '#' || ctype == 24) {
      chunks[current_chunk].colourtype = 24;
      newcolour24(chunks[current_chunk].colour.colour24);
      chunks[current_chunk].colour.colour24[BCID] = 0;
      hexto24(chunks[current_chunk].colour.colour24, NULL, colourarg);
    }

    else {
      fprintf(stderr, "Invalid colour: %s\n\n", colourarg);
      HELP;
    }
    current_chunk++;
    arg_index += 2;
  }

  while (fgets(buf, sizeof buf, stdin) != NULL) {
    int i = 0;
    colourize(buf, begin, chunks, chunk_count);
    buf[i] = '\0';
  }
}
