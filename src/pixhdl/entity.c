#include "entity.h"


/**
 * Creates an Entity object.
 * @return ent (Entity *): Newly created Entity object
 */
Entity * createEntity ()
{
    // Allocate memory for an entity
    Entity * ent = calloc(1, sizeof(Entity));

    // Initialize all the signal pointers to NULL
    ent->signals_in = NULL;
    ent->signals_out = NULL;
    ent->signals_inout = NULL;

    // Initialize all the counters to 0
    ent->count_in = 0;
    ent->count_out = 0;
    ent->count_inout = 0;
    ent->max_name_size = 0;

    // Return the Entity object
    return ent;
}


/**
 * Frees the memory of an Entity object.
 * @param ent (Entity *): Entity object to be freedß
 */
void destroyEntity (Entity * ent)
{
    size_t i = 0;

    if (ent) {

        for (i = 0; i < ent->count_in; i++)
            if (ent->signals_in[i].name)
                free(ent->signals_in[i].name);

        for (i = 0; i < ent->count_out; i++)
            if (ent->signals_out[i].name)
                free(ent->signals_out[i].name);

        for (i = 0; i < ent->count_inout; i++)
            if (ent->signals_inout[i].name)
                free(ent->signals_inout[i].name);

        if (ent->signals_in) free(ent->signals_in);
        if (ent->signals_out) free(ent->signals_out);
        if (ent->signals_inout) free(ent->signals_inout);

        free(ent);
    }
}


/**
 * Adds a Signal object to an Entity object.
 * @param  ent (Entity *): Entity object to add Signal object to
 * @param  sig (Signal *): Signal object to be added to Entity object
 * @return 0 if successful, 1 if not successful
 */
int addSignalToEntity (Entity * ent, const Signal * sig)
{
    // Pointer to Signals array (helps generalize initialization)
    Signal ** signals = NULL;
    // Pointer to count integer (helps generalize initialization)
    unsigned int * count = NULL;

    // Choose the right Signals array & counter integer
    // based on the direction of the signal
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
            // This shouldn't happen
            check(NULL, "Unknown signal direction (%d).", sig->dir);
    }


    // Reallocate memory for the signal array based on count,
    // we want to add another element to the signals array
    *signals = realloc(*signals, (*count + 1) * sizeof(Signal));
    // Check if realloc was successful
    check_mem(signals);

    // Copy the memory of the Signal object into the newly allocated space
    memcpy((Signal *) (*signals + *count), sig, sizeof(Signal));
    // Increment the signal counter
    (*count)++;

    // Update the maximum name size if needed
    if (strlen(sig->name) > ent->max_name_size)
        ent->max_name_size = strlen(sig->name);

    return 0;


error:
    // Something went wrong
    return 1;
}
