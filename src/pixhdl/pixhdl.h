#ifndef __PIXHDL_H__
#define __PIXHDL_H__

#define VERSION "1.0.0"
#define HELP_SCREEN_TEXT "\
Usage: pixhdl [options] <VHDL file>\n\
\n\
    options:\n\
           -o : specify output filename (can also come after VHDL file argument)\n\
           -v : print the version number\n\
           -h : print this help screen"

#include <unistd.h>
#include "dbg.h"
#include "entity.h"
#include "print_entity.h"
#include "parser.h"
#include "generate_svg.h"


/**
 * Prints a help screen containing the available optional arguments.
 * @return (int): always returns 0
 */
int print_help_screen ();


#endif
