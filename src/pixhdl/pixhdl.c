#include "pixhdl.h"


int main (int argc, char * argv[])
{
    Entity * ent = createEntity();

    Signal sig = { .dir = IN, .length = 32, .name = "CLK" };
    Signal sig2 = { .dir = INOUT, .length = 1, .name = "Very Long Signal Name" };

    addSignalToEntity(ent, &sig);
    addSignalToEntity(ent, &sig2);

    printEntitySignals(ent);

    destroyEntity(ent);

    return 0;
}
