/*
    Important definitions for project wide usage
*/

#include <libclr/libclr.h>

#ifndef COLOURIZERDEF
#define COLOURIZERDEF

// One chunk consists of three arguments --[after|from|...] <string> <colour>
typedef struct {
  enum { FROM, AFTER, RESETON } type;
  char *match;
  int colourtype;
  union {
    colour24 colour24;
    colour8 colour8;
    colour4 colour4;
  } colour;
} chunk;

void colourize(const char *str, chunk begin, chunk *chunks, int chunk_count);

#endif /*COLOURIZERDEF*/
