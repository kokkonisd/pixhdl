#include "entity.h"


/**
 * Creates an Entity object.
 * @return ent (Entity *): Newly created Entity object
 */
Entity * createEntity ()
{
    // Allocate memory for an entity
    Entity * ent = calloc(1, sizeof(Entity));

    // Initialize name to NULL
    ent->name = NULL;

    // Initialize generic inputs to NULL
    ent->generics = NULL;

    // Initialize all the signal pointers to NULL
    ent->signals_in = NULL;
    ent->signals_out = NULL;
    ent->signals_inout = NULL;

    // Initialize all the counters to 0
    ent->count_in = 0;
    ent->count_out = 0;
    ent->count_inout = 0;

    ent->max_name_size_generics = 0;
    ent->max_name_size_in = 0;
    ent->max_name_size_out = 0;
    ent->max_name_size_inout = 0;
    ent->max_name_size_global = 0;

    ent->max_length_size_generics = 0;
    ent->max_length_size_in = 0;
    ent->max_length_size_out = 0;
    ent->max_length_size_inout = 0;
    ent->max_length_size_global = 0;

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
        for (i = 0; i < ent->count_generics; i++) {
            if (ent->generics[i].name)
                free(ent->generics[i].name);
            if (ent->generics[i].length)
                free(ent->generics[i].length);
        }

        for (i = 0; i < ent->count_in; i++) {
            if (ent->signals_in[i].name)
                free(ent->signals_in[i].name);
            if (ent->signals_in[i].length)
                free(ent->signals_in[i].length);
        }

        for (i = 0; i < ent->count_out; i++) {
            if (ent->signals_out[i].name)
                free(ent->signals_out[i].name);
            if (ent->signals_out[i].length)
                free(ent->signals_out[i].length);
        }

        for (i = 0; i < ent->count_inout; i++) {
            if (ent->signals_inout[i].name)
                free(ent->signals_inout[i].name);
            if (ent->signals_inout[i].length)
                free(ent->signals_inout[i].length);
        }

        if (ent->generics) free(ent->generics);
        if (ent->signals_in) free(ent->signals_in);
        if (ent->signals_out) free(ent->signals_out);
        if (ent->signals_inout) free(ent->signals_inout);

        if (ent->name) free(ent->name);

        free(ent);
    }
}


/**
 * Adds a Signal object to an Entity object.
 * @param  ent (Entity *): Entity object to add Signal object to
 * @param  sig (Signal *): Signal object to be added to Entity object
 * @return          (int): 0 if successful, 1 if not successful
 */
int addSignalToEntity (Entity * ent, const Signal * sig)
{
    // Pointer to Signals array (helps generalize initialization)
    Signal ** signals = NULL;
    // Pointer to count integer (helps generalize initialization)
    unsigned int * count = NULL;
    size_t * max_name_size = NULL;
    size_t * max_length_size = NULL;

    // Choose the right Signals array & counter integer
    // based on the direction of the signal
    switch (sig->dir) {
        case IN:
            signals = &ent->signals_in;
            count = &ent->count_in;
            max_name_size = &ent->max_name_size_in;
            max_length_size = &ent->max_length_size_in;
            break;

        case OUT:
            signals = &ent->signals_out;
            count = &ent->count_out;
            max_name_size = &ent->max_name_size_out;
            max_length_size = &ent->max_length_size_out;
            break;

        case INOUT:
            signals = &ent->signals_inout;
            count = &ent->count_inout;
            max_name_size = &ent->max_name_size_inout;
            max_length_size = &ent->max_length_size_inout;
            break;

        case GENERIC:
            signals = &ent->generics;
            count = &ent->count_generics;
            max_name_size = &ent->max_name_size_generics;
            max_length_size = &ent->max_length_size_generics;
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
    if (strlen(sig->name) > *max_name_size)
        *max_name_size = strlen(sig->name);

    if (strlen(sig->length) > *max_length_size)
        *max_length_size = strlen(sig->length);

    if (strlen(sig->name) > ent->max_name_size_global)
        ent->max_name_size_global = strlen(sig->name);

    if (strlen(sig->length) > ent->max_length_size_global)
        ent->max_length_size_global = strlen(sig->length);

    return 0;


error:
    // Something went wrong
    return 1;
}
