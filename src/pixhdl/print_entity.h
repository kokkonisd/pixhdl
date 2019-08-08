/**
 * @file print_entity.h
 * @author Dimitri Kokkonis ([\@kokkonisd](https://github.com/kokkonisd))
 * @date 4 Aug 2019
 * @brief In-terminal printing operations for the _pixhdl_ project.
 */


#ifndef __PRINT_ENTITY_H__
#define __PRINT_ENTITY_H__

#include "entity.h"


/**
 * Convert a direction enum to string.
 *
 * @param  dir (`direction`): The direction to be converted.
 * @return  (`const char *`): The string equivalent of the direction.
 */
const char * directionToString (direction dir);

/**
 * Prints all the signals of an Entity in the following order: `IN`, `OUT`,
 * `INOUT`.
 *
 * @param ent (`Entity *``): The Entity object whose signals are to be printed.
 */
void printEntitySignals (Entity * ent);


#endif
