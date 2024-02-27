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
      opt = get_option(argv[arg_index]);
    } else if (arg_index == 1) {
      give_colour(&begin, argv[1]);
      continue;
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
      printf("Colourizer \033[31m" CLRVERSION "\033[0m\n");
      exit(0);
    default:
      fprintf(stderr, "Unknow option: %s\n\n", argv[arg_index]);
    case 'h':
      HELP;
      break;
    }
    // Next arg is matching string
    if (arg_index + 1 >= argc) {
      fprintf(stderr, "No string specified\n\n");
      HELP;
    }
    chunks[current_chunk].match = argv[arg_index + 1];

    chunk_init(&chunks[current_chunk]);

    // Reset-on takes no colour argument
    if (chunks[current_chunk].type == RESETON) {
      current_chunk++;
      arg_index++;
      continue;
    }

    // Next to next argument is the colour
    if (arg_index + 2 >= argc) {
      fprintf(stderr, "No colour specified\n\n");
      HELP;
    }
    char *colourarg = argv[arg_index + 2];
    give_colour(&chunks[current_chunk], colourarg);
    current_chunk++;
    arg_index += 2;
  }

  while (fgets(buf, sizeof buf, stdin) != NULL) {
    int i = 0;
    colourize(buf, begin, chunks, chunk_count);
    buf[i] = '\0';
  }
}
