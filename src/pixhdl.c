#include "pixhdl.h"


Entity * createEntity ()
{
    Entity * ent = calloc(1, sizeof(Entity));
    ent->count = 0;

    return ent;
}


void destroyEntity (Entity * ent)
{
    if (ent && ent->signals) free(ent->signals);

    free(ent);
}


int addSignalToEntity (Entity * ent, const Signal * sig)
{
    ent->signals = realloc(ent->signals, (ent->count + 1) * sizeof(Signal));
    check_mem(ent->signals);

    memcpy((Signal *) (ent->signals + ent->count), sig, sizeof(Signal));
    ent->count++;

    return 0;

error:
    return 1;
}


int main (int argc, char * argv[])
{
    printf("This is pixhdl.\n");

    Entity * ent = createEntity();

    Signal sig = { .dir = IN, .length = 32, .name = "This is it" };

    addSignalToEntity(ent, &sig);

    printf("`%s` has a length of %d.\n", ent->signals[0].name, ent->signals[0].length);

    destroyEntity(ent);

    return 0;
}
