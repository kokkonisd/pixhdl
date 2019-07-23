#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"


typedef enum {
    IN,
    OUT,
    INOUT
} direction;

typedef struct {
    char * name;
    direction dir;
    unsigned int length;
} Signal;

typedef struct {
    Signal * signals_in;
    Signal * signals_out;
    Signal * signals_inout;

    unsigned int count_in;
    unsigned int count_out;
    unsigned int count_inout;

    size_t max_name_size;
} Entity;


Entity * createEntity ();
void destroyEntity (Entity * ent);
int addSignalToEntity (Entity * ent, const Signal * sig);


#endif
