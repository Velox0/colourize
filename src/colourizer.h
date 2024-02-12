/*
    Important definitions for project wide usage
*/

#include <libclr/libclr.h>

#ifndef COLOURIZERDEF
#define COLOURIZERDEF

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

#endif /*COLOURIZERDEF*/
