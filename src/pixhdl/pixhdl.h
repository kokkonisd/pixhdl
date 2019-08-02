#ifndef __PIXHDL_H__
#define __PIXHDL_H__

#define VERSION "1.1.1"
#define HELP_SCREEN_TEXT "\
Usage: pixhdl [options] <VHDL file>\n\
\n\
    options:\n\
           -o, --output  : specify output filename\n\
           -p, --print   : print the entity in the terminal (doesn't produce an output image file)\n\
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
 * @return (int): always returns 0
 */
int print_help_screen ();


#endif
