#include "optionhandler.h"
#include "colourizer.h"
#include <libclr/colourmods.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char getoption(const char *opt) {
  if (opt[1] != '-')
    return opt[1];

  char *long_options[] = {"after",    "from",    "afterx", "fromx",
                          "reset-on", "version", "help"};
  char short_options[] = "afAFrvh";
  int HOWMANYOPTIONS = sizeof(short_options) / sizeof(char) - 1;

  for (int i = 0; i < HOWMANYOPTIONS; i++) {
    for (int j = 2; opt[j]; j++) {
      if (opt[j] != long_options[i][j - 2])
        break;
      if (!opt[j + 1] && !long_options[i][j - 1])
        return short_options[i];
    }
  }
  return '\0';
}

void help(const char *arg) {
  char red[] = "\033[0;31m";
  char green[] = "\033[0;32m";
  char normal[] = "\033[0;0m";

  printf("Usage: %scommand %s|%s %s %s[OPTIONS]%s\n", green, red, green, arg,
         red, normal);

  printf("  -a, --after    \t"
         "Apply <colour> formatting to text after matching <string>\n");
  printf("  -f, --from     \t"
         "Apply <colour> formatting to text starting from the match of <string>"
         "\n");
  printf("  -r, --reset-on \t"
         "Reset text colour to normal after matching <string>\n");
  printf("  Note: Combining short options is not supported!\n");
  exit(0);
}

void give_colour(chunk *the, const char *colour) {
  char *colour_list[] = {"black",        "red",
                         "green",        "yellow",
                         "blue",         "magenta",
                         "cyan",         "white",
                         "bright_black", "bright_red",
                         "bright_green", "bright_yellow",
                         "bright_blue",  "bright_magenta",
                         "bright_cyan",  "bright_white"};

  for (int i = 0; i < 16; i++) {
    if (!strcmp(colour, colour_list[i])) {
      the->colourtype = 4;
      the->colour.colour4 = getcolour4(0, i);
      return;
    }
  }

  the->colourtype = 24;
  the->colour.colour24[BCID] = 0;
  newcolour24(the->colour.colour24);
  resetbg24(the->colour.colour24);
  int stat = hexto24(the->colour.colour24, NULL, colour);
  if (stat == -1) {
    the->colourtype = -1;
    fprintf(stderr, "Invalid colour: %s\n\n", colour);
  }
}
