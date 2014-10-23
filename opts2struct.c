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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opts2struct.h"

#define MAXARGLEN 256
#define OPT2S_MIN(x, y) ((x < y) ? x : y)

static char **opts2struct_splitstring(const char *cstring, const char *sep) {
  char *dupstring = calloc(strlen(cstring) + 1, 1);
  char *cptr = (char *)cstring, *dptr = dupstring;
  while (*cptr != '\0') {
    if (*cptr != '\'' && *cptr != '"') {
      *dptr = *cptr;
      ++dptr;
    }
    ++cptr;
  }
  char **splitarray = malloc(nopts * sizeof(char *));

  char *word, *ctx, *space;
  int i = 0;
  for (word = strtok_r(dupstring, sep, &ctx); word;
       word = strtok_r(NULL, sep, &ctx)) {
    assert(i < nopts);
    splitarray[i++] = (space = strrchr(word, ' ')) ? space + 1 : word;
  }
  return splitarray;
}

struct opts2struct_t *opts2struct_create(void) {
  struct opts2struct_t *opts2struct = malloc(sizeof(struct opts2struct_t));
  memset(opts2struct->found, 0, nopts * sizeof(int));
  char **names = opts2struct_splitstring(opts2s_allopts, ",");
  for (int i = 0; i < nopts; ++i) {
    opts2struct->names[i] = names[i];
  }
  free(names);
  return opts2struct;
}

static char **opts2struct_argv_filled(struct opts2struct_t *ops2s,
                                      const int argc, const char *argv[]) {
  char **defaults = opts2struct_splitstring(opts2s_alldefaults, ",");
  int *optindex = malloc(nopts * sizeof(int));
  int foundopts = 0;
  for (int i = 0; i < nopts; ++i) {
    size_t longestmatch = 0;
    optindex[i] = OPTS2EMPTY;
    for (int j = 0; j < argc; ++j) {
      char *first = strstr(argv[j], ops2s->names[i]);
      if (first) {
        size_t match = first - ops2s->names[i];
        if (match > longestmatch) {
          longestmatch = match;
          optindex[i] = j;
          ++foundopts;
        }
      }
    }
  }
  int skip_args = argc - foundopts;
  char **argv_filled = malloc((nopts + skip_args) * sizeof(char *));
  for (int i = 0; i < nopts; ++i) {
    if (OPTS2EMPTY == optindex[i]) {
      argv_filled[i] =
          malloc(strlen(defaults[i]) + strlen(ops2s->names[i]) + 2);
      sprintf(argv_filled[i], "%s=%s", ops2s->names[i], defaults[i]);
      ops2s->v[i] = defaults[i];
    } else {
      argv_filled[i] = strdup(argv[optindex[i]]);
    }
  }
  free(optindex);
  return argv_filled;
}

void opts2struct_parseopts(struct opts2struct_t *optstruct, const int argc,
                           const char *argv[]) {
  char **new_argv = opts2struct_argv_filled(optstruct, argc, argv);
  char key[MAXARGLEN];
  const char *stringvalue;
  // TODO: only need one loop. Leftover from when had no defaults, used argc
  for (int i = 0; i < nopts; ++i) {
    size_t arglen = OPT2S_MIN(strlen(new_argv[i]), MAXARGLEN);
    size_t k = 0;
    while (k < arglen && new_argv[i][k] == '-') {
      ++k;
    }
    int m = 0;
    while (k < arglen && new_argv[i][k] != '=') {
      key[m++] = new_argv[i][k++];
    }
    key[m] = '\0';

    opts2s_argtype_t argtype = opts2s_argtype_numargtypes;
    int intvalue;
    float floatvalue;
    int is_flag = (k == arglen) || strstr(&new_argv[i][k + 1], "true") ||
                  strstr(&new_argv[i][k + 1], "false");
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
      stringvalue = &new_argv[i][k + 1];
      char *firstbad = NULL;
      intvalue = (int)strtol(stringvalue, &firstbad, 10);
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
