#include "entity.h"


Entity * createEntity ()
{
    Entity * ent = calloc(1, sizeof(Entity));
    ent->count = 0;
    ent->max_name_size = 0;

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

    if (strlen(sig->name) > ent->max_name_size)
        ent->max_name_size = strlen(sig->name);

    return 0;

error:
    return 1;
}
