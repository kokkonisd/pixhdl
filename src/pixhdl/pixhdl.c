#include "pixhdl.h"


int main (int argc, char * argv[])
{
    Entity * ent = getEntityFromFile("tests/vhdl_sources/ALU.vhdl");
    check(ent, "Couldn't get entity from file.");

    printEntitySignals(ent);

    destroyEntity(ent);

    return 0;

error:
    return 1;
}
