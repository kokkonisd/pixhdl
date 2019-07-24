#include "pixhdl.h"


int main (int argc, char * argv[])
{
    Entity * ent = getEntityFromFile("tests/vhdl_sources/a1_poolingLayer.vhd");
    check(ent, "Couldn't get entity from file.");

    printEntitySignals(ent);

    destroyEntity(ent);

    return 0;

error:
    return 1;
}
