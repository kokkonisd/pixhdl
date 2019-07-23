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
} Direction;

typedef struct {
    Direction dir;
    unsigned int length;
    char * name;
} Signal;

typedef struct {
    Signal * signals;
    unsigned int count;
    size_t max_name_size;
} Entity;


Entity * createEntity ();
void destroyEntity (Entity * ent);
int addSignalToEntity (Entity * ent, const Signal * sig);


#endif
