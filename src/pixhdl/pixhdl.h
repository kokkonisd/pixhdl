/**
 * @file pixhdl.h
 * @author Dimitri Kokkonis ([\@kokkonisd](https://github.com/kokkonisd))
 * @date 4 Aug 2019
 * @brief Main application file for the _pixhdl_ project.
 */


#ifndef __PIXHDL_H__
#define __PIXHDL_H__

/** Current version of _pixhdl_ (in `X.Y.Z` format). */
#define VERSION "1.1.1"

/** Text for the help screen of _pixhdl_, detailing usage and possible options. */
#define HELP_SCREEN_TEXT "\
Usage: pixhdl [options] <VHDL file>\n\
\n\
    options:\n\
           -o, --output  : specify output filename\n\
           -p, --print   : print the entity in the terminal\n\
           -u, --update  : check for updates (and update if newer version is available)\n\
           -v, --version : print the version number\n\
           -h, --help    : print this help screen"


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "dbg.h"
#include "entity.h"
#include "print_entity.h"
#include "parser.h"
#include "generate_svg.h"
#include "auto_update.h"


/**
 * Prints a help screen containing the available optional arguments.
 *
 * @return (`int`): Always returns 0.
 */
int print_help_screen ();


#endif
