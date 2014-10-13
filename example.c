/*
 example.c

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

int main(int argc, const char *argv[]) {

  OPTS2STRUCT(argc, argv, subjects, voxels, blocks, trs, offsets);

  printf("in a loop:\n");
  int i;
  for (i = 0; i < nopts; ++i) {
    printf("%s : %d\n", ops2s.names[i], ops2s.v[i]);
  }
  printf("\nusing struct fields:\n");
  printf("subjects: %d\n", ops2s.subjects);
  printf("voxels: %d\n", ops2s.voxels);
  printf("blocks: %d\n", ops2s.blocks);
  printf("trs: %d\n", ops2s.trs);
  printf("offsets: %d\n", ops2s.offsets);

  printf("\nusing enums in array:\n");
  printf("subjects: %d\n", ops2s.v[subjects]);
  printf("voxels: %d\n", ops2s.v[voxels]);
  printf("blocks: %d\n", ops2s.v[blocks]);
  printf("trs: %d\n", ops2s.v[trs]);
  printf("offsets: %d\n", ops2s.v[offsets]);

  return 0;
}
