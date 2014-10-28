//
//  opts2struct_utils.h
//
//  Created by Benjamin Singer on 10/27/14.
//  Copyright (c) 2014 Benjamin Singer.
//  http://opensource.org/licenses/MIT
//

#ifndef __opts2struct_utils__
#define __opts2struct_utils__

#include <stdlib.h>

/**
 * Example usage:
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
 *filled
 *                  with results
 *  @param leninfo  an nsubstr-length array of lengths to be filled with
 *results
 *  @param evensep  separator + characters to ignore on even substring
 *indices
 *                  (0, 2, 4 ...)
 *                   if NULL, will use default which is ", \"'"
 *                   e.g, split once at the start of any amount of adjacent
 *                   commas, spaces, single quotes, double quotes
 *                   and don't include them in the substrings.
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

#endif /* defined(__opts2struct_utils__) */
