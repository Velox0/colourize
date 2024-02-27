#ifndef CLROPTIONS
#define CLROPTIONS
#include "colourizer.h"

char getoption(const char *opt);
void help(const char *arg);

void give_colour(chunk *the, const char *colour);

#endif