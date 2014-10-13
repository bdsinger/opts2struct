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

You can parse command-lines of the form:

    command stars=100 bunnies=7 dogs=4

or the more conventional:

    command --stars=100 --bunnies=7 --dogs=4

- order is not important
- no limit on number of key-value pairs (except shell limits)

provided you have 

    OPTS2STRUCT(argc, argv, stars, bunnies, dogs);

in your main function, resulting in

    enum {
      stars = 0
      bunnies = 1
      dogs = 2
      nopts = 3
    };
    
    opts2struct_t ops2s;
    
    ops2s.stars = 100;
    ops2s.bunnies = 7;
    ops2s.dogs = 4;
    
    ops2s.v[0] = 100;
    ops2s.v[1] = 7;
    ops2s.v[2] = 4;
    
    ops2s.names[0] = "stars";
    ops2s.names[1] = "bunnies";
    ops2s.names[2] = "dogs";
    
    /* equivalently ... */
    
    ops2s.v[stars] = 100;
    ops2s.v[bunnies] = 7;
    ops2s.v[dogs] = 4;
    
    ops2s.names[stars] = "stars";
    ops2s.names[bunnies] = "bunnies";
    ops2s.names[dogs] = "dogs";
  

Note that the values are limited to ints

example
------
See the [example.c](https://github.com/bdsinger/opts2struct/blob/master/example.c) file for an example.

issues:
------
- must have equal sign separating key and value
- not spaces (`--bunnies 7` won't work)

