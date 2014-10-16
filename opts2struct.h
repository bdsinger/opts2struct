/*
opts2struct.h

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

#ifndef H_OPTS2STRUCT
#define H_OPTS2STRUCT

#define OPTS2STRUCT(opts...)                                                   \
  enum { opts, nopts };                                                        \
  struct opts2struct_t {                                                       \
    union {                                                                    \
      int v[nopts];                                                            \
      struct {                                                                 \
        int opts;                                                              \
      };                                                                       \
    };                                                                         \
    const char *names[nopts];                                                  \
  };                                                                           \
  static inline const char *opts2s_allopts(void) { return #opts; }

/*
 * must include your particular "myopts2struct.h" here
 *    - below OPTS2STRUCT definition
 *                &&
 *    - above opts2struct_* function prototypes
 *
 *    it must contain at least these 2 types of lines:
 *       OPTS2STRUCT(structfield1,structfield2,...,structfieldN)
 *       #define OPTS2EMPTY -9999
 *
 *    where the name and number of the struct fields are up to you, and
 *    the value of OPTS2EMPTY is up to you but:
 *         - must be an int, and
 *         - must be a value that the user would never set for any field
 *
 *    see "myopts2struct.h.example"
 *
 *   (note that .gitignore is set to ignore "myopts2struct.h" since it is
 *    in essence a config file: user- and project- specific)
 */

#include "myopts2struct.h"

/**
 *  opts2struct_create creates an opts2struct as customized in myopts2struct.h
 *  @return struct opts2struct_t*
 */
struct opts2struct_t *opts2struct_create(void);

/**
 *  opts2struct_parseopts parses argv into an initialized opts2struct
 *
 *  @param optstruct created and initialized by opts2struct_create()
 *  @param argc      number of command line arguments from your main()
 *  @param argv      command line arguments from your main()
 */
void opts2struct_parseopts(struct opts2struct_t *optstruct, int argc,
                           const char *argv[]);

#endif // H_OPTS2STRUCT
