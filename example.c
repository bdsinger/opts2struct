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
 * myopts2struct.h! This example assumes:
 *
 *   OPTS2STRUCT(stars, bunnies, dogs)
 */

int main(int argc, const char *argv[]) {

  /*
   command line examples

   WORKS :

   command stars=100 bunnies=7 dogs=4
   command -stars=100 --bunnies=7 dogs
   command  bunnies=brown stars=4.5 -nodogs
   command  -bunnies stars=4.5
   command --stars bunnies=2 thiswillbeignored


   WON'T WORK:

   command bunnies 10 dogs 4000
   command -bunnies 10 --dogs 4000

  ->because an equals sign '=' must be used to set the value,
        not spaces!

   */

  struct opts2struct_t *ops2s = opts2struct_create();
  opts2struct_parseopts(ops2s, argc, argv);

  printf("in a loop:\n");
  for (int i = 0; i < nopts; ++i) {
    printf("%s : ", ops2s->names[i]);
    switch (ops2s->argtypes[i]) {
    case opts2s_argtype_bool:
      printf("%s (bool)", ops2s->v[i]);
      break;
    case opts2s_argtype_cstringarg:
      printf("%s (string)", ops2s->v[i]);
      break;
    case opts2s_argtype_intarg:
      printf("%d (int)", ops2s->i[i]);
      break;
    case opts2s_argtype_floatarg:
      printf("%f (float)", ops2s->f[i]);
      break;
    default:
      printf("unknown argtype %d!", ops2s->argtypes[i]);
      break;
    }
    printf("\n");
  }

  printf("\nusing struct fields:\n");
  printf("stars: %s\n", ops2s->stars);
  printf("bunnies: %s\n", ops2s->bunnies);
  printf("dogs: %s\n", ops2s->dogs);

  printf("\nusing enums in array:\n");
  printf("stars: %s\n", ops2s->v[stars]);
  printf("bunnies: %s\n", ops2s->v[bunnies]);
  printf("dogs: %s\n", ops2s->v[dogs]);

  return 0;
}
