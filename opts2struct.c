/*
opts2struct.c

part of opts2struct project: https://github.com/bdsinger/opts2struct

The MIT License (MIT)

Copyright (c) 2014 Benjamin D Singer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>
#include "opts2struct.h"

struct opts2struct_t *opts2struct_create(void) {
  char *optstring = strdup(opts2s_allopts());
  struct opts2struct_t *optstruct = malloc(sizeof(struct opts2struct_t));
  char *word, *ctx, *space;
  int i = 0;
  for (word = strtok_r(optstring, ",", &ctx); word;
       word = strtok_r(NULL, ",", &ctx)) {
    optstruct->names[i++] = (space = strrchr(word, ' ')) ? space + 1 : word;
  }
  free(optstring);
  return optstruct;
}

void opts2struct_parseopts(struct opts2struct_t *optstruct, int argc,
                           const char *argv[]) {
  for (int j = 0; j < nopts; ++j) {
    optstruct->v[j] = OPTS2EMPTY;
  }
  for (int i = 1; i < argc; ++i) {
    for (int j = 0; j < nopts; ++j) {
      if (strstr(argv[i], optstruct->names[j])) {
        optstruct->v[j] = atoi(strrchr(argv[i], '=') + 1);
        break;
      }
    }
  }
}
