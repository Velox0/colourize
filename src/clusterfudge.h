#ifndef CLUSTERFUDGE
#define CLUSTERFUDGE
#include "colourizer.h"

void sort(int *index, int *chunk_index, int matches);

// position of matching string
int match(chunk chunk, const char *str, int start, int f);

bool is_different_colour(chunk A, chunk B);

#endif // CLUSTERFUDGE
