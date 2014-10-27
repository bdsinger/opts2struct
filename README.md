opts2struct
===========

A C macro with helper functions for creating an opts struct from argv

Usage
-----

- Step 1: provide your option names, defaults, and short names in `myopts2struct.h`:

    `OPTS2STRUCT(key1, key2, key3, ... , keyN)`
    `OPTS2STRUCT(default_val1, default_val2, default_val3, ..., default_valN)`
    `OPTS2STRUCT(shortkey1, shortkey2, shortkey3, ..., shortkeyN)`

    - this file is treated as a config file and is in .gitignore so it isn't tracked.
    - make a copy of `myopts2struct.h.example` to start. Note that you also need to set `OPTS2EMPTY` in there. 

- Step 2: call helper functions to initialize and then parse `argv[]`:

    `struct opts2struct_t *ops2s = opts2struct_create();`

    `opts2struct_parseopts(ops2s, argc, argv);`

    - this step is optional if you'd rather do the parsing yourself, but then the entire thing is optional, isn't it?


Details
-------

After calling `opts2struct_create()` in Step 2 above, you'll have auto-generated structure field names in `ops2s`, as well as `enum`s matching the names of your keys for accessing the same options using an array. You'll also have `names[]`, `defaults[]`, and `shortnames[]` arrays containing your field names, defaults, and shortnames as C strings.

After supplying `opts2struct_parseopts()` with the argv C string array of `--key=value`, `--key`, or `--nokey` items (there can be 0 or more dashes to start the key name within the input) or `-k value`, `-k true`, or `-nok` you'll have:

- `ops2s->found[]` to see which options were found on the command line
- `ops2s->argtypes[]` to get their type (C string, integer, floating point, or boolean in the case of `--[no]key`)
- `ops2s->i[]` and `ops2s->as_int->` to access integer-valued options (or boolean, which have `0` and `1` integer values)
- `ops2s->f[]` and `ops2s->as_float->` to access floatingpoint-valued options (as well as pre-promoted integers and booleans)
- `ops2s->v[]` and `ops2s->` to access values in their original string form, split off from argv (or `true` and `false` in the case of boolean flag input)

Defaults and Short Names
--------
    `OPTS2DEFAULTS(default1, default2, ..., defaultN)` and
    `OPTS2SHORTNAMES(shortname1, shortname2, ..., shortnameN)`

- allow setting default values for keys not provided by user
- shortnames can be provided with spaces rather than equal signs between key and value
    - e.g., `-s 3` (the single hyphen is required, unlike the key=value form)
- `N` above must be equal to the number of options in the `OPTS2STRUCT` macro
- all three macro settings should be made in `myopts2struct.h`

Example
------

See the [example.c](https://github.com/bdsinger/opts2struct/blob/master/example.c) file for an example.

Run `make test` to run a set of different command-line invocations, demontrating defaults, short names, and bool flags.


Limitations:
------

- opts2struct is intentionally simple, and is not meant to replace [getopt](http://en.wikipedia.org/wiki/Getopt)
- probably incompatible with Visual C++, but should work with minor tweaks
    - `opts2struct.h` uses [named variadic macros](https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros) (a GNU extension to C99 always-named-`__VA_ARGS__` variadic macros) and [anonymous structs and unions](https://gcc.gnu.org/onlinedocs/gcc/Unnamed-Fields.html), which are part of the C11 standard and a GNU extension prior to that (for unions anyway).

The following limitations apply to the helper functions:

- no attempt to differentiate between option and non-option area of command-line (ie no `--`), 
  but args not listed in OPTS2STRUCT macro will be ignored
- required supplying default values and shortnames, and creates static global counterparts
    - the only way to bypass them is to ifdef out the lines that refer to them
    - next todo item is different levels of "buy-in"
- workaround: just copy the OPTS2STRUCT macro to a new file and use that alone
    - perhaps remove the fields for names, defaults, and shortnames if not desired
    - do your own parsing etc.
 

