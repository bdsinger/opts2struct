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

typedef const char *opts2s_cstring;
typedef enum {
  opts2s_argtype_bool,
  opts2s_argtype_intarg,
  opts2s_argtype_floatarg,
  opts2s_argtype_cstringarg,
  opts2s_argtype_numargtypes
} opts2s_argtype_t;

#define OPTS2STRUCT(opts...)                                                   \
  enum { opts, nopts };                                                        \
  struct opts2struct_t {                                                       \
    union {                                                                    \
      int i[nopts];                                                            \
      struct {                                                                 \
        int opts;                                                              \
      } as_int;                                                                \
    };                                                                         \
    union {                                                                    \
      float f[nopts];                                                          \
      struct {                                                                 \
        float opts;                                                            \
      } as_float;                                                              \
    };                                                                         \
    union {                                                                    \
      opts2s_cstring v[nopts];                                                 \
      struct {                                                                 \
        opts2s_cstring opts;                                                   \
      };                                                                       \
    };                                                                         \
    const char *names[nopts];                                                  \
    const char *defaults[nopts];                                               \
    const char *shortnames[nopts];                                             \
    opts2s_argtype_t argtypes[nopts];                                          \
    int found[nopts];                                                          \
  };                                                                           \
  static const char *opts2s_allopts = #opts;

#define OPTS2DEFAULTS(opts_defaults...)                                        \
  static const char *opts2s_alldefaults = #opts_defaults;

#define OPTS2SHORTNAMES(snames...)                                             \
  static const char *opts2s_allshortnames = #snames;

/*
 * must include your particular "myopts2struct.h" here
 *    - below OPTS2STRUCT definition
 *                &&
 *    - above opts2struct_* function prototypes
 *
 *    it must contain at least these 3 types of lines:
 *       OPTS2STRUCT(structfield1,structfield2,...,structfieldN)
 *       OPTS2DEFAULTS(default1,default2,...,defaultN)
 *       OPTS2SHORTNAMES(sname1,sname2,...,snameN)
 *       #define OPTS2EMPTY -9999
 *
 *    where the name and number of the struct fields are up to you, and
 *    the number of defaults & shortnames must match the number of fields.
 *
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
 *  and splits allopts, alldefaults, and allshortnames into:
 *     opts2struct->names[], ->defaults, and ->shortnames[] resp.
 *  @return struct opts2struct_t*
 */
struct opts2struct_t *opts2struct_create(void);

/**
 *  opts2struct_parseopts parses argv into an initialized opts2struct
 *
 *  @param optstruct created and initialized by opts2struct_create()
 *  @param argc      number of command line arguments from your main()
 *  @param argv      command line arguments from your main()
 *
 */
void opts2struct_parseopts(struct opts2struct_t *optstruct, const int argc,
                           const char *argv[]);

/* Below here are utility functions used by the above functions, provided
   in the public API in case they are useful */

/**
 * Example usage of the utility functions opts2struct_splitstring_info and
   opts2struct_strings_from_info:
 *
 *  {
      const char *string="-stars -3.5 -cat 3 -mouse 2";
      char* outstrings[6];
      int pos[6], loc[6];

      // remove option hyphen on even idx, but could be negative sign on odd
      opts2struct_splitstring_info(string, 6, pos, loc, "-, ", ", ");
      opts2struct_strings_from_info(string, 6, pos, loc, outstrings);
      ...
      // remember to free the new strings
      for (int i=0; i<6; i++) {
          free(outstrings[i]);
      }
   }
    // or to use basic split with default separator string:
    {
      const char* string="one, two, three";
      int nsub;
      char **outstrings  = opts2struct_basicsplit(string, &nsub);
      ...
     // remember to free the new strings
     for (int i=0; i<nsub; i++) {
        free(outstrings[i]);
     }
     free(outstrings);
    }
 */

/**
 *  opts2struct_splitstring_info provides info needed to split a string
 *
 *  @param instring string to split
 *  @param nsubstr  number of substrings in instring
 *  @param posinfo  an nsubstr-length array of substring positions to be
 *                  filled with results
 *  @param leninfo  an nsubstr-length array of lengths to be filled with
 *                  results
 *  @param evensep  separator + characters to ignore on even substring
 *                  indices (0, 2, 4 ...). If NULL, will use default 
 *                  which is ", \"'" e.g, split once at the start of any
 *                  amount of adjacent commas, spaces, single quotes,
 *                  double quotes and don't include them in the substrings.
 *  @param oddsep   separator + characters to ignore on
 *                  odd substring indices (1, 3, 5 ...)
 *                   if NULL, will use evensep
 *  @return the actual number of substrings found
 */
int opts2struct_splitstring_info(const char *instring, const int nsubstr,
                                 size_t *posinfo, size_t *leninfo,
                                 const char *evensep, const char *oddsep);

/**
 *  opts2struct_strings_from_info
 *
 *  @param instring   string that was split
 *  @param nsubstr    number of substrings in instring
 *  @param posinfo    start positions, filled out by
 *                    opts2struct_splitstring_info
 *  @param leninfo    lengths, filled out by opts2struct_splitstring_info
 *  @param outstrings the substrings themselves
 *
 *    @warning caller must free each substring in outstrings
 */
void opts2struct_strings_from_info(const char *instring, const int nsubstr,
                                   const size_t *posinfo, const size_t *leninfo,
                                   char **outstrings);

/**
 *  opts2struct_basicsplit
 *
 *  @param instring  string to split
 *  @param nsubstr number of substrings found (if not NULL)
 *
 *  @return nsubstr-length array of substrings
 *  @warning caller must substrings array returned
 */
char **opts2struct_basicsplit(const char *instring, int *nsubstr);

#endif // H_OPTS2STRUCT
