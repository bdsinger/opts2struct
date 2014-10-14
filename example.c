/*
example.c

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

#include <stdio.h>
#include "opts2struct.h"

/* IMPORTANT:
 * the names of the struct fields here *must match those you set up in
 * myopts2struct.h!
 */

int main(int argc, const char *argv[]) {

  /* run command-line like so (order of key-value pairs not important, and
     the only restriction on values is that they be integers):

   command subjects=10 voxels=40000 blocks=10 trs=12 offsets=5

   Note that the current version of the parsing code in opts2struct.c just
   looks for the opts name *within* the command line arguments, so you should
   use unambiguous option names that do not include any other arguments as
   substrings!

   WON'T WORK:
    command s=1 k=2 ss=4

       -> because both "s" and "ss" contain an "s"!

   WON'T WORK:
    command subjects 10 voxels 4000

       -> because an equals sign '=' must be used to set the value,
          not spaces!

   */

  struct opts2struct_t *ops2s = opts2struct_create();
  opts2struct_parseopts(ops2s, argc, argv);

  printf("in a loop:\n");
  for (int i = 0; i < nopts; ++i) {
    printf("%s : %d\n", ops2s->names[i], ops2s->v[i]);
  }
  printf("\nusing struct fields:\n");
  printf("stars: %d\n", ops2s->stars);
  printf("bunnies: %d\n", ops2s->bunnies);
  printf("dogs: %d\n", ops2s->dogs);

  printf("\nusing enums in array:\n");
  printf("stars: %d\n", ops2s->v[stars]);
  printf("bunnies: %d\n", ops2s->v[bunnies]);
  printf("dogs: %d\n", ops2s->v[dogs]);

  return 0;
}
