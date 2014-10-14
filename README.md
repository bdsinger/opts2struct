opts2struct
===========

A short C macro and two helper functions for creating an opts struct from argv

- Step 1: provide your option names in `myopts2struct.h`

    `OPTS2STRUCT(key1, key2, key3, ...)`

- Step 2: call helper functions to initialize and then parse `argv[]`:

    `struct opts2struct_t *ops2s = opts2struct_create();`
    `opts2struct_parseopts(myopts, argc, argv);`

Technical details
-----------------

Provided you have edited `myopts2struct.h` to contain:

    OPTS2STRUCT(stars, bunnies, dogs)

then the following will be automatically generated:

    enum {
      stars = 0,
      bunnies = 1,
      dogs = 2,
      nopts = 3
    };
    
    struct opts2struct_t {
      union {
        int v[nopts];
        struct {
           int stars, bunnies, dogs;
        };
      };
      const char* names[nopts];
    };

    static inline const char *opts2s_allopts(void) { return "stars, bunnies, dogs"; }


The `opts2s_allopts()` function returns the stringified options, split by `opts2struct_create()` when filling out the `names[]` array

After calling `opts2struct_create()` you'll have the following set:
    
    ops2s->names[stars]   /*  "stars"   */
    ops2s->names[bunnies] /*  "bunnies" */
    ops2s->names[dogs]    /*  "dogs"    */
    ops2s->stars          /*  OPTS2EMPTY */
    ops2s->bunnies        /*  OPTS2EMPTY */
    ops2s->dogs           /*  OPTS2EMPTY */
    ops2s->v[stars]       /*  OPTS2EMPTY */
    ops2s->v[bunnies]     /*  OPTS2EMPTY */
    ops2s->v[dogs]        /*  OPTS2EMPTY */

Note that you can also just loop over `ops2s->v[i]` using `nopts` in a for-loop, which requires no knowledge of the option names (outside of `myopts2struct.h`).

After calling `opts2struct_parseopts(ops2s, argc, argv)` with the
command-line `command stars=100 bunnies=7 dogs=4`:

    ops2s->stars          /*  100 */
    ops2s->bunnies        /*  7 */
    ops2s->dogs           /*  4 */
    ops2s->v[stars]       /*  100 */
    ops2s->v[bunnies]     /*  7 */
    ops2s->v[dogs]        /*  4 */

Two features of the [C preprocessor](https://gcc.gnu.org/onlinedocs/cpp/index.html#Top) make this possible:
- [stringification](https://gcc.gnu.org/onlinedocs/cpp/Stringification.html#Stringification)
    - see `#opts` on [line 41 of opts2struct.h](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L41) (later used by `opts2struct_create()` in [opts2struct.c](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.c)
- [named variadic macros](https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros)
    - see `opts...` on [line 30 of opts2struct.h](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L30), and [lines 31](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L31) and [36](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L36)  where it expands into the enums, and struct fields respectively.
    - while variadic macros are part of the C99 standard, the _named_ variant of variadic macros is a GNU extension supported on Linux and OS X but not some other platforms-- you'd need to change `opts...` to `...` in Visual C++ as well as replace `opts` in the body with `__VA_ARGS__`

The code also uses [anonymous structs and unions](https://gcc.gnu.org/onlinedocs/gcc/Unnamed-Fields.html), which are part of the C11 standard, but work on earlier versions of the gcc compiler as an extension. Without them, `ops2s.stars` would become something like `ops2s.u.s.stars` and `ops2s.v[stars]` would also need an extra field for the union, say `ops2s.u.v[stars]`. Ugly. Compiling on pre-C11 compilers is left as an exercise for the reader.

Example
------
See the [example.c](https://github.com/bdsinger/opts2struct/blob/master/example.c) file for an example.

Limitations:
------
opts2struct is intentionally simple, and is not meant to replace [getopt](http://en.wikipedia.org/wiki/Getopt): 
- the values are limited to ints
- must have equal sign separating key and value
- not spaces (`--bunnies 7` won't work)
- no mechanism for default values
    - but you can check for items set to OPTS2EMPTY, meaning unset, then set them to defaults
- opts are matched if they form __any substring__ of __any element of argv[]__!
    - use long unambiguous option names that do not contain each other as substrings!
    - if "dog" is an option, then it matches both "dog=3" and "bigdog=4" command line arguments
- no attempt to differentiate between option and non-option area of command-line (ie no `--`)
- no flag support (ie no `-a -b` or `-ab`)
- incompatible with Visual C++, but should work with minor tweaks (see above)

