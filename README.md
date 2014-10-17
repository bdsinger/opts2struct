opts2struct
===========

A short C macro with helper functions for creating an opts struct from argv

Usage
-----

- Step 1: provide your option names in `myopts2struct.h`:

    `OPTS2STRUCT(key1, key2, key3, ...)`

    - this file is treated as a config file and is in .gitignore so it isn't tracked.
    - make a copy of `myopts2struct.h.example` to start. Note that you also need to set `OPTS2EMPTY` in there. 

- Step 2: call helper functions to initialize and then parse `argv[]`:

    `struct opts2struct_t *ops2s = opts2struct_create();`

    `opts2struct_parseopts(ops2s, argc, argv);`

    - this step is optional if you'd rather do the parsing yourself, but then the entire thing is optional, isn't it?


Details
-------

After calling `opts2struct_create()` in Step 2 above, you'll have auto-generated structure field names in `ops2s`, as well as `enum`s matching the names of your keys for accessing the same options using an array. You'll also have a `names[]` array containing your field names as C strings.

After supplying `opts2struct_parseopts()` with the argv C string array of `--key=value`, `--key`, or `--nokey` items (there can be 0 or more dashes to start the key name within the input)  you'll have:

- `ops2s->found[]` to see which options were found on the command line
- `ops2s->argtypes[]` to get their type (C string, integer, floating point, or boolean in the case of `--[no]key`)
- `ops2s->i[]` and `ops2s->as_int->` to access integer-valued options (or boolean, which have `0` and `1` integer values)
- `ops2s->f[]` and `ops2s->as_float->` to access floatingpoint-valued options (as well as pre-promoted integers and booleans)
- `ops2s->v[]` and `ops2s->` to access values in their original string form, split off from argv (or `true` and `false` in the case of boolean flag input)


Example
------

See the [example.c](https://github.com/bdsinger/opts2struct/blob/master/example.c) file for an example.


Limitations:
------

- opts2struct is intentionally simple, and is not meant to replace [getopt](http://en.wikipedia.org/wiki/Getopt)
- probably incompatible with Visual C++, but should work with minor tweaks
    - `opts2struct.h` uses [named variadic macros](https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros) (a GNU extension to C99 always-named-`__VA_ARGS__` variadic macros) and [anonymous structs and unions](https://gcc.gnu.org/onlinedocs/gcc/Unnamed-Fields.html), which are part of the C11 standard and a GNU extension prior to that (for unions anyway).

The following limitations apply to the helper functions:

- requires equal sign separating key and value, not spaces (so `-pillows=7`works, but `-pillows 7` will not)
- no mechanism for default values
    - but you can check for items set to OPTS2EMPTY, or check the `found` list, and set any unset values to defaults
- no attempt to differentiate between option and non-option area of command-line (ie no `--`)

