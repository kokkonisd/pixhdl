#ifndef __PIXHDL_H__
#define __PIXHDL_H__

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
    uint32_t length;
    char * name;
} Signal;

typedef struct {
    Signal * signals;
    uint32_t count;
} Entity;


Entity * createEntity ();
void destroyEntity (Entity * ent);
int addSignalToEntity (Entity * ent, const Signal * sig);

#endif
