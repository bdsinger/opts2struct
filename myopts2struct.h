/* this file should only be included by opts2struct.h
 * after the definition of OPTS2STRUCT but before the
 * function prototypes beginning with opts2struct_
 */

/**
 *  OPTS2STRUCT(<comma-separated keys>) : set your options here. The
 *                         OPTS2STRUCT macro in opts2struct.h will
 *                         auto-generate a structure definition from it.
 *
 *      limitation: no option should be a substring of any other option!
 */
OPTS2STRUCT(stars, bunnies, dogs)

/**
 *  OPTS2EMPTY : opts2struct_create() pre-fills the int-valued fields/v[] of
 *               the struct prior to parsing input arguments. If an option
 *               has this value, the assumption is that the user did not
 *               supply this value on the command line.
 *
 *      limitation: must be a valid "int". Also, be sure it is a value
 *               that is outside the range of valid values for all options!
 */
#define OPTS2EMPTY -999
