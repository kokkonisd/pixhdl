/**
 * @file entity.h
 * @author Dimitri Kokkonis ([\@kokkonisd](https://github.com/kokkonisd))
 * @date 4 Aug 2019
 * @brief Entity definitions & basic operations for the _pixhdl_ project.
 */


#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"


/**
 * @brief Enumeration of the possible directions of a signal.
 *
 * The possible directions of a signal (port) are IN, OUT
 * and INOUT. However, an entity can also have GENERIC inputs;
 * thinking of GENERIC as an additional direction helps generalize
 * the code (adding a Signal to an Entity, printing an Entity etc).
 */
typedef enum {
    IN,
    OUT,
    INOUT,
    GENERIC
} direction;

/**
 * @brief Definition of the Signal object.
 *
 * A Signal is simply an input/output/throughput of an Entity.
 * It can be a port (IN, OUT, INOUT) or a generic input (GENERIC).
 * It has a name, a direction and a length (or value, in the case
 * of generic inputs).
 */
typedef struct {
    char * name;         /**< Name of the signal.                        */
    direction dir;       /**< Direction of the signal.                   */
    char * length;       /**< Length of the signal (in bits).
                              It's a string because the parser
                              will attempt to calculate its integer
                              value, but will leave it as-is if it fails
                              (for example if it contains generics).     */
} Signal;

/**
 * @brief Definition of the Entity object.
 *
 * An Entity is a collection of input, output, throughput and
 * generic input signals. It also has a name, and -- in order
 * to facilitate later operations such as image generation
 * and printing -- also contains the maximum lengths of both
 * port names and port sizes (for each type of port).
 */
typedef struct {
    char * name;                     /**< Name of the entity.                                    */

    Signal * generics;               /**< Generic inputs.                                        */
    Signal * signals_in;             /**< Array of IN signals.                                   */
    Signal * signals_out;            /**< Array of OUT signals.                                  */
    Signal * signals_inout;          /**< Array of INOUT signals.                                */

    unsigned int count_generics;     /**< Length of the `generics` array.                        */
    unsigned int count_in;           /**< Length of the `signals_in` array.                      */
    unsigned int count_out;          /**< Length of the `signals_out` array.                     */
    unsigned int count_inout;        /**< Length of the `signals_inout` array.                   */

    size_t max_name_size_generics;   /**< Maximum size of the name of a generic input.           */
    size_t max_name_size_in;         /**< Maximum size of the name of an IN signal.              */
    size_t max_name_size_out;        /**< Maximum size of the name of an OUT signal.             */
    size_t max_name_size_inout;      /**< Maximum size of the name of an INOUT signal.           */
    size_t max_name_size_global;     /**< Maximum size of the name of a signal in this Entity.   */

    size_t max_length_size_generics; /**< Maximum size of the length of a generic input.         */
    size_t max_length_size_in;       /**< Maximum size of the length of an IN signal.            */
    size_t max_length_size_out;      /**< Maximum size of the length of an OUT signal.           */
    size_t max_length_size_inout;    /**< Maximum size of the length of an INOUT signal.         */
    size_t max_length_size_global;   /**< Maximum size of the length of a signal in this Entity. */
} Entity;


/**
 * Creates an Entity object.
 *
 * @return ent (`Entity *`): Newly created Entity object.
 */
Entity * createEntity ();

/**
 * Frees the memory of an Entity object.
 *
 * @param ent (`Entity *`): Entity object to be freed.
 */
void destroyEntity (Entity * ent);

/**
 * Adds a Signal object to an Entity object.
 *
 * @param  ent (`Entity *`): Entity object to add Signal object to.
 * @param  sig (`Signal *`): Signal object to be added to Entity object.
 * @return          (`int`): 0 if successful, 1 if not successful.
 */
int addSignalToEntity (Entity * ent, const Signal * sig);


#endif
