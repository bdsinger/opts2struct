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

#define MAXARGLEN 256
#define OPT2S_MIN(x, y) ((x < y) ? x : y)

struct opts2struct_t *opts2struct_create(void) {
  static char *opts2s_allopts_dup = NULL;
  if (opts2s_allopts_dup == NULL) {
    opts2s_allopts_dup = strdup(opts2s_allopts);
  }
  char *optstring = opts2s_allopts_dup;

  struct opts2struct_t *optstruct = malloc(sizeof(struct opts2struct_t));
  memset(optstruct->found, 0, nopts * sizeof(int));
  char *word, *ctx, *space;
  int i = 0;
  for (word = strtok_r(optstring, ",", &ctx); word;
       word = strtok_r(NULL, ",", &ctx)) {
    optstruct->names[i++] = (space = strrchr(word, ' ')) ? space + 1 : word;
  }
  return optstruct;
}

void opts2struct_parseopts(struct opts2struct_t *optstruct, int argc,
                           const char *argv[]) {
  char key[MAXARGLEN];
  const char *stringvalue;
  for (int i = 0; i < argc; ++i) {
    // const char *stringvalue = strrchr(argv[i], '=');
    size_t arglen = OPT2S_MIN(strlen(argv[i]), MAXARGLEN);
    int k = 0;
    while (k < arglen && argv[i][k] == '-') {
      ++k;
    }
    int m = 0;
    while (k < arglen && argv[i][k] != '=') {
      key[m++] = argv[i][k++];
    }
    key[m] = '\0';

    opts2s_argtype_t argtype = opts2s_argtype_numargtypes;
    int intvalue;
    float floatvalue;
    int is_flag = (k == arglen);
    if (is_flag) {
      argtype = opts2s_argtype_bool;
      if ('n' == key[0] && 'o' == key[1]) {
        intvalue = 0;
        for (int n = 0; n <= m - 2; ++n) {
          key[n] = key[n + 2];
        }
        stringvalue = "false";
      } else {
        intvalue = 1;
        stringvalue = "true";
      }
      floatvalue = (float)intvalue;
    } else {
      stringvalue = &argv[i][k + 1];
      char *firstbad = NULL;
      intvalue = (int)strtol(stringvalue, &firstbad, 10);
      floatvalue = (float)OPTS2EMPTY;
      if (firstbad != NULL && *firstbad != '\0') {
        floatvalue = strtof(stringvalue, &firstbad);
        argtype = (firstbad == stringvalue) ? opts2s_argtype_cstringarg
                                            : opts2s_argtype_floatarg;
      } else {
        argtype = opts2s_argtype_intarg;
        floatvalue = (float)intvalue;
      }
    }

    for (int j = 0; j < nopts; ++j) {
      if (optstruct->found[j]) {
        continue;
      }
      optstruct->i[j] = OPTS2EMPTY;
      optstruct->f[j] = (float)OPTS2EMPTY;
      optstruct->v[j] = NULL;
      if (!strcmp(key, optstruct->names[j])) {
        optstruct->v[j] = stringvalue;
        optstruct->i[j] = intvalue;
        optstruct->f[j] = floatvalue;
        optstruct->argtypes[j] = argtype;
        optstruct->found[j] = 1;
        break;
      }
    }
  }
}
