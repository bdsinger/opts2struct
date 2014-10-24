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

static char **opts2struct_splitstring(const char *cstring, const char *sep,
                                      const char *removechars) {
  // get a copy of input string without the
  // undesirable characters (quotes, mainly)
  char **splitarray = NULL;
  char *dupstring = NULL;
  size_t nremove = strlen(removechars);
  size_t nunsplit = strlen(cstring);
  if (nunsplit > 0) {
    dupstring = malloc(strlen(cstring));

    int k = 0;
    for (size_t i = 0; i < nunsplit; ++i) {
      int skip = 0;
      for (size_t j = 0; j < nremove; ++j) {
        if (cstring[i] == removechars[j]) {
          skip = 1;
          break;
        }
      }
      if (skip)
        continue;
      dupstring[k++] = cstring[i];
    }
    dupstring[k] = '\0';

    // split the copy at the given separator and return
    splitarray = malloc(nopts * sizeof(char *));
    char *word, *ctx, *space;
    int i = 0;
    for (word = strtok_r(dupstring, sep, &ctx); word;
         word = strtok_r(NULL, sep, &ctx)) {
      assert(i < nopts);
      splitarray[i++] = (space = strrchr(word, ' ')) ? space + 1 : word;
    }
  }
  return splitarray;
}

struct opts2struct_t *opts2struct_create(void) {
  // create the struct
  struct opts2struct_t *opts2struct = malloc(sizeof(struct opts2struct_t));
  memset(opts2struct->found, 0, nopts * sizeof(int));

  // parse the option names
  char **names = opts2struct_splitstring(opts2s_allopts, ",", "\"'-");
  for (int i = 0; i < nopts; ++i) {
    opts2struct->names[i] = names[i];
  }
  free(names);
  return opts2struct;
}

void opts2struct_parseopts(struct opts2struct_t *optstruct, const int argc,
                           const char *argv[]) {
  // load the defaults
  char **defaults = opts2struct_splitstring(opts2s_alldefaults, ",", "\"'");

  // set the values from either argv or defaults or set OPTS2EMPTY
  for (int i = 0; i < nopts; ++i) {
    int hasdefault = (NULL != defaults[i]);

    // go through argv
    char key[MAXARGLEN];
    strcpy(key, optstruct->names[i]);

    const char *stringvalue = NULL;
    opts2s_argtype_t argtype = opts2s_argtype_numargtypes;
    int intvalue = OPTS2EMPTY;
    float floatvalue = (float)OPTS2EMPTY;
    int negflag = 0;

    for (int j = 0; j < argc; ++j) {
      char **keyvaluepair = opts2struct_splitstring(argv[j], "=", "\"'-");
      char *argkey = keyvaluepair[0];
      stringvalue = keyvaluepair[1];

      // see if this option was on command line
      int offset = 0;
      if ((NULL == stringvalue) && argkey[0] == 'n' && argkey[1] == 'o') {
        negflag = 1;
        offset = 2;
      }
      if (!strcmp(&argkey[offset], key)) {
        optstruct->found[i] = 1;
        break;
      }
      negflag = 0;
    }

    if (0 == optstruct->found[i] && hasdefault) {
      stringvalue = defaults[i];
    }

    // if key has no value, is prob a flag but
    // check defaults (if any) since flag=true is a valid default setting
    int novalue = (NULL == stringvalue);
    int is_flag = novalue || negflag || !strcmp(stringvalue, "true") ||
                  !strcmp(stringvalue, "false");
    if (is_flag) {
      argtype = opts2s_argtype_bool;
      if (negflag) {
        intvalue = 0;
        if (novalue)
          stringvalue = "false";
      } else {
        intvalue = 1;
        if (novalue)
          stringvalue = "true";
      }
      floatvalue = (float)intvalue;
    } else {
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

    optstruct->argtypes[i] = argtype;
    optstruct->v[i] = stringvalue;
    optstruct->i[i] = intvalue;
    optstruct->f[i] = floatvalue;
  }
}