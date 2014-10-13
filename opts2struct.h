/*
  opts2struct.h

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

#ifndef H_OPTS2STRUCT
#define H_OPTS2STRUCT

#include <stdlib.h>
#include <string.h>

#define OPTS2STRUCT(argc, argv, opts...)                                       \
  enum { opts, nopts };                                                        \
  typedef struct {                                                             \
    char names[nopts][256];                                                    \
    union {                                                                    \
      struct {                                                                 \
        int opts;                                                              \
      };                                                                       \
      int v[nopts];                                                            \
    };                                                                         \
  } ops2struct_t;                                                              \
  ops2struct_t ops2s = {0};                                                    \
  do {                                                                         \
    char *word, *ctx, *space, tmp[256], *optstring = #opts;                    \
    strcpy(tmp, optstring);                                                    \
    int j, i = 0;                                                              \
    for (word = strtok_r(tmp, ",", &ctx); word;                                \
         word = strtok_r(NULL, ",", &ctx)) {                                   \
      strcpy(ops2s.names[i++],                                                 \
             (space = strrchr(word, ' ')) ? space + 1 : word);                 \
    }                                                                          \
    for (i = 1; i < argc; ++i) {                                               \
      for (j = 0; j < nopts; ++j) {                                            \
        if (strstr(argv[i], ops2s.names[j])) {                                 \
          ops2s.v[j] = atoi(strrchr(argv[i], '=') + 1);                        \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif // H_OPTS2STRUCT
