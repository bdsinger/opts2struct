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

#ifndef OPTS2STRUCT
#error OPTS2STRUCT not defined!
#endif

static inline int empty_or_null(const char *str) {
  return (str == NULL || 0 == strlen(str));
}

static inline void free_before_setting(const char **ptr, const char *setting) {
  if (NULL != *ptr) {
    free((void *)*ptr);
  }
  *ptr = setting;
}

static void opts2struct_splitstring_info(const char *instring,
                                         const int nsubstr, size_t *posinfo,
                                         size_t *leninfo, const char *evensep,
                                         const char *oddsep) {
  assert(instring);
  assert(posinfo);
  assert(leninfo);

  // default sep: a comma-delimited, possibly-quoted,
  // possibly-space-containing instring
  const char *defaultsep = ",'\" ";
  const char *myeven = (NULL == evensep) ? defaultsep : evensep;
  const char *myodd = (NULL == oddsep) ? myeven : oddsep;

  int pos = 0;
  for (int i = 0; i < nsubstr; i++) {
    const char *thissep = (i % 2) ? myodd : myeven;
    size_t skip = strspn(&instring[pos], thissep);
    pos += skip;
    posinfo[i] = pos;
    leninfo[i] = strcspn(&instring[pos], thissep);
    pos += leninfo[i];
  }
}

static void opts2struct_strings_from_info(const char *instring,
                                          const int nsubstr,
                                          const size_t *posinfo,
                                          const size_t *leninfo,
                                          char **outstrings) {
  assert(instring);
  assert(posinfo);
  assert(leninfo);
  assert(outstrings);

  for (int i = 0; i < nsubstr; i++) {
    outstrings[i] = malloc(leninfo[i] + 1);
    strncpy(outstrings[i], &instring[posinfo[i]], leninfo[i]);
    outstrings[i][leninfo[i]] = '\0';
  }
}

struct opts2struct_t *opts2struct_create(void) {
  // create the struct
  struct opts2struct_t *opts2struct = malloc(sizeof(struct opts2struct_t));
  memset(opts2struct->found, 0, nopts * sizeof(int));

  size_t posinfo[nopts];
  size_t leninfo[nopts];

  // load the names
  opts2struct_splitstring_info(opts2s_allopts, nopts, posinfo, leninfo,
                               ",'\" -", NULL);
  opts2struct_strings_from_info(opts2s_allopts, nopts, posinfo, leninfo,
                                (char **)opts2struct->names);

  // load the defaults, if any
  opts2struct_splitstring_info(opts2s_alldefaults, nopts, posinfo, leninfo,
                               NULL, NULL);
  opts2struct_strings_from_info(opts2s_alldefaults, nopts, posinfo, leninfo,
                                (char **)opts2struct->defaults);

  // load the short names, if any
  opts2struct_splitstring_info(opts2s_allshortnames, nopts, posinfo, leninfo,
                               ",'\" -", NULL);
  opts2struct_strings_from_info(opts2s_allshortnames, nopts, posinfo, leninfo,
                                (char **)opts2struct->shortnames);

  return opts2struct;
}

void opts2struct_parseopts(struct opts2struct_t *optstruct, const int argc,
                           const char *argv[]) {
  int *argmatched = calloc(argc, sizeof(int));

  // set the values from either argv or defaults or set OPTS2EMPTY
  for (int i = 0; i < nopts; i++) {
    int hasdefault = (NULL != optstruct->defaults[i]);

    // go through argv
    const char *stringvalue = NULL;
    opts2s_argtype_t argtype = opts2s_argtype_numargtypes;
    int intvalue = OPTS2EMPTY;
    float floatvalue = (float)OPTS2EMPTY;
    int negflag = 0;

    char *keyvaluepair[2];
    for (int j = 0; j < argc; j++) {
      if (argmatched[j]) {
        continue;
      }
      // see if this option was on command line
      size_t posinfo[nopts];
      size_t leninfo[nopts];
      opts2struct_splitstring_info(argv[j], 2, posinfo, leninfo, "=\"'-",
                                   "=\"'");
      opts2struct_strings_from_info(argv[j], 2, posinfo, leninfo, keyvaluepair);
      const char *argkey = keyvaluepair[0];
      stringvalue = keyvaluepair[1];
      // flag check
      int offset = 0;
      if (empty_or_null(stringvalue) && argkey[0] == 'n' && argkey[1] == 'o') {
        negflag = 1;
        offset = 2;
      }
      // short name check
      if (argv[j][0] == '-' && !strcmp(&argkey[offset], optstruct->shortnames[i])) {
        if (negflag) {
          stringvalue = "false";
        } else {
          stringvalue = argv[j + 1];
        }
        optstruct->found[i] = 1;
        argmatched[j] = 1;
        break;
      }
      // name check
      if (!strcmp(&argkey[offset], optstruct->names[i])) {
        optstruct->found[i] = 1;
        argmatched[j] = 1;
        break;
      }
      free((void *)argkey);
      free_before_setting(&stringvalue, NULL);
      negflag = 0;
    }

    if (0 == optstruct->found[i] && hasdefault) {
      free_before_setting(&stringvalue, optstruct->defaults[i]);
    }

    // if key has no value, is prob a flag but
    // check defaults (if any) since flag=true is a valid default setting
    int novalue = empty_or_null(stringvalue);
    int is_flag = novalue || negflag || !strcmp(stringvalue, "true") ||
                  !strcmp(stringvalue, "false");
    if (is_flag) {
      argtype = opts2s_argtype_bool;
      if (negflag) {
        intvalue = 0;
        if (novalue) {
          free_before_setting(&stringvalue, "false");
        }
      } else {
        intvalue = 1;
        if (novalue) {
          free_before_setting(&stringvalue, "true");
        }
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
  free((void *)argmatched);
}