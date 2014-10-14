opts2struct
===========

A 30-line C macro that builds and fills a struct from a set of key=integer_value command line options.


In your main file, include "opts2struct.h" and in the main function add a single line:

    OPTS2STRUCT(argc, argv, key1, key2, key3);

You'll now have the following available to you (local to your main function):


struct fields:
--------------

access values via key names:

    ops2s.key1
    ops2s.key2
    ops2s.key3

or via a value array (good in loops):

    ops2s.v[key1]
    ops2s.v[key2]
    ops2s.v[key3]

there is also a names array of key names:

    ops2s.names[key1]
    ops2s.names[key2]
    ops2s.names[key3]

the arrays work since you have the following enums:

    key1 = 0,
    key2 = 1,
    key3 = 2,
    nopts = 3

technical details
-----------------

provided you have 

    OPTS2STRUCT(argc, argv, stars, bunnies, dogs);

in your main function, the following will be automatically generated:

    enum {
      stars = 0
      bunnies = 1
      dogs = 2
      nopts = 3
    };
    
    typedef struct {
      char names[nopts][256];
      union {
        struct {
           int stars, bunnies, dogs;
        };
        int v[nopts];
      };
    } opts2struct_t;
    
    opts2struct_t ops2s;
    
    ops2s.names[stars] = "stars";
    ops2s.names[bunnies] = "bunnies";
    ops2s.names[dogs] = "dogs";

    /* equivalently:  */
    
    ops2s.names[0] = "stars";
    ops2s.names[1] = "bunnies";
    ops2s.names[2] = "dogs";

Two features of the [C preprocessor](https://gcc.gnu.org/onlinedocs/cpp/index.html#Top) make this possible:
- [stringification](https://gcc.gnu.org/onlinedocs/cpp/Stringification.html#Stringification)
    - see `#opts` on [line 46 of opts2struct.h](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L46) where it is expanded and parsed into the `.names[]` array field
- [named variadic macros](https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros)
    - see `opts...` on [line 33 of opts2struct.h](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L33), and [lines 34](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L34) and [39](https://github.com/bdsinger/opts2struct/blob/master/opts2struct.h#L39)  where it expands into the enums, and struct fields respectively.
    - while variadic macros are part of the C99 standard, the _named_ variant of variadic macros is a GNU extension supported on Linux and OS X but not some other platforms-- you'd need to change `opts...` to `...` in Visual C++ as well as replace `opts` in the body with `__VA_ARGS__`

The code also uses anonymous structs and unions, which are part of the C11 standard, but work on earlier versions of the gcc compiler as an extension. Without them, `ops2s.stars` would become something like `ops2s.u.s.stars` and `ops2s.v[stars]` would also need an extra field for the union, say `ops2s.u.v[stars]`. Ugly. Compiling on pre-C11 compilers is left as an exercise for the reader.

example command-line input
--------------------------

- order is not important
- no limit on number of key-value pairs (except shell limits)

You can parse command-lines of the form:

    command stars=100 bunnies=7 dogs=4

or the more conventional:

    command --stars=100 --bunnies=7 --dogs=4

resulting in:

    /* given the example command line input: */

    ops2s.stars = 100;
    ops2s.bunnies = 7;
    ops2s.dogs = 4;
    
    ops2s.v[stars] = 100;
    ops2s.v[bunnies] = 7;
    ops2s.v[dogs] = 4;
    
    /* equivalently */
    
    ops2s.v[0] = 100;
    ops2s.v[1] = 7;
    ops2s.v[2] = 4;
    
    
example
------
See the [example.c](https://github.com/bdsinger/opts2struct/blob/master/example.c) file for an example.

limitations:
------
opts2struct is intentionally simple, and is not meant to replace [getopt](http://en.wikipedia.org/wiki/Getopt): 
- the values are limited to ints
- must have equal sign separating key and value
- not spaces (`--bunnies 7` won't work)
- no mechanism for default values
- opts are matched if they are a subset of args: assumes unique word args
- no attempt to differentiate between option and non-option area of command-line (ie no `--`)
- no flag support (ie no `-a -b` or `-ab`)
- incompatible with Visual C++, but should work with minor tweaks (see above)
