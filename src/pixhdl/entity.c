#include "entity.h"


Entity * createEntity ()
{
    Entity * ent = calloc(1, sizeof(Entity));
    ent->count_in = 0;
    ent->count_out = 0;
    ent->count_inout = 0;
    ent->signals_in = NULL;
    ent->signals_out = NULL;
    ent->signals_inout = NULL;
    ent->max_name_size = 0;

    return ent;
}


void destroyEntity (Entity * ent)
{
    if (ent) {
        if (ent->signals_in) free(ent->signals_in);
        if (ent->signals_out) free(ent->signals_out);
        if (ent->signals_inout) free(ent->signals_inout);

        free(ent);
    }
}


int addSignalToEntity (Entity * ent, const Signal * sig)
{
    Signal ** signals = NULL;
    unsigned int * count = NULL;

    switch (sig->dir) {
        case IN:
            signals = &ent->signals_in;
            count = &ent->count_in;
            break;
        case OUT:
            signals = &ent->signals_out;
            count = &ent->count_out;
            break;
        case INOUT:
            signals = &ent->signals_inout;
            count = &ent->count_inout;
            break;
        default:
            check(NULL, "Unknown signal direction (%d).", sig->dir);
    }


    *signals = realloc(*signals, (*count + 1) * sizeof(Signal));
    check_mem(signals);

    memcpy((Signal *) (*signals + *count), sig, sizeof(Signal));
    (*count)++;

    if (strlen(sig->name) > ent->max_name_size)
        ent->max_name_size = strlen(sig->name);

    return 0;

error:
    return 1;
}
