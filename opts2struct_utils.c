//
//  opts2struct_utils.c
//
//  Created by Benjamin Singer on 10/27/14.
//  Copyright (c) 2014 Benjamin Singer.
//  http://opensource.org/licenses/MIT
//
#include <assert.h>
#include <string.h>
#include "opts2struct_utils.h"

int opts2struct_splitstring_info(const char *instring, const int nsubstr,
                                 size_t *posinfo, size_t *leninfo,
                                 const char *evensep, const char *oddsep) {
  assert(instring);
  assert(posinfo);
  assert(leninfo);

  memset(posinfo, 0, sizeof(size_t) * nsubstr);
  memset(leninfo, 0, sizeof(size_t) * nsubstr);

  // default sep: a comma-delimited, possibly-quoted,
  // possibly-space-containing instring
  const char *defaultsep = ",'\" ";
  const char *myeven = (NULL == evensep) ? defaultsep : evensep;
  const char *myodd = (NULL == oddsep) ? myeven : oddsep;

  int found_nsubstr = 0;
  int pos = 0;
  for (int i = 0; i < nsubstr; i++) {
    const char *thissep = (i % 2) ? myodd : myeven;
    size_t skip = strspn(&instring[pos], thissep);
    pos += skip;
    posinfo[i] = pos;
    size_t len = strcspn(&instring[pos], thissep);
    if (0 == len) {
      break;
    }
    leninfo[i] = len;
    pos += leninfo[i];
    found_nsubstr++;
  }
  return found_nsubstr;
}

void opts2struct_strings_from_info(const char *instring, const int nsubstr,
                                   const size_t *posinfo, const size_t *leninfo,
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

char **opts2struct_basicsplit(const char *instring, int *nsubstr) {
  assert(instring);
  const int maxsubs = 1024;
  size_t pos[maxsubs], loc[maxsubs];
  char **outstrings = NULL;

  int nsub =
      opts2struct_splitstring_info(instring, maxsubs, pos, loc, NULL, NULL);
  if (nsub > 0) {
    outstrings = malloc(nsub * sizeof(char *));
    opts2struct_strings_from_info(instring, maxsubs, pos, loc,
                                  (char **)outstrings);
  }
  if (nsubstr) {
    *nsubstr = nsub;
  }
  return outstrings;
}
