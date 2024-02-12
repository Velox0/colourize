#include "colourizer.h"
#include "optionhandler.h"
#include "version.h"
#include <libclr/libclr.h>
#include <stdio.h>
#include <stdlib.h>

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
      } else
        fprintf(stderr, "Invalid colour: %s\n\n", argv[1]);
      continue;
    } else {
      fprintf(stderr, "Invalid parameter: %s\n\n", argv[arg_index]);
      help(argv[0]);
    }
    switch (opt) {
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
      fprintf(stderr, "Unknow option: %s\n\n", argv[arg_index]);
    case 'h':
      help(argv[0]);
      break;
    }
    // Next arg is matching string
    chunks[current_chunk].match = argv[arg_index + 1];

    // Reset-on takes no colour argument
    if (chunks[current_chunk].type == RESETON) {
      current_chunk++;
      arg_index++;
      continue;
    }
    // Next to next argument is the colour
    int clr = whichcolour(argv[arg_index + 2]);
    if (clr > 0) {
      chunks[current_chunk].colour.colour4 = getcolour4(0, arg_index);
    } else {
      fprintf(stderr, "Invalid colour: %s\n\n", argv[arg_index + 2]);
      help(argv[0]);
    }
    current_chunk++;
    arg_index += 2;
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
