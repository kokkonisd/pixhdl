#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"


// Define the possible directions of a signal
typedef enum {
    IN,
    OUT,
    INOUT
} direction;

// Define the Signal object
typedef struct {
    char * name;         // Name of the signal
    direction dir;       // Direction of the signal
    unsigned int length; // Length of the signal (in bits)
} Signal;

// Define the Entity object
typedef struct {
    char * name;              // Name of the entity

    Signal * signals_in;      // Array of IN signals
    Signal * signals_out;     // Array of OUT signals
    Signal * signals_inout;   // Array of INOUT signals

    unsigned int count_in;    // Length of the signals_in array
    unsigned int count_out;   // Length of the signals_out array
    unsigned int count_inout; // Length of the signals_inout array

    size_t max_name_size;     // Maximum length of the name of a signal
                              // in this Entity
} Entity;


/**
 * Creates an Entity object.
 * @return ent (Entity *): Newly created Entity object
 */
Entity * createEntity ();

/**
 * Frees the memory of an Entity object.
 * @param ent (Entity *): Entity object to be freedß
 */
void destroyEntity (Entity * ent);

/**
 * Adds a Signal object to an Entity object.
 * @param  ent (Entity *): Entity object to add Signal object to
 * @param  sig (Signal *): Signal object to be added to Entity object
 * @return 0 if successful, 1 if not successful
 */
int addSignalToEntity (Entity * ent, const Signal * sig);


#endif
