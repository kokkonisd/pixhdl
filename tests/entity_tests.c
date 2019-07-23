#include "minunit.h"
#include <pixhdl/entity.h>
#include <assert.h>


char * create_and_destroy_entity ()
{
    Entity * ent = createEntity();
    mu_assert(ent, "Entity could not be created.");

    destroyEntity(ent);

    return NULL;
}


char * add_in_signal_to_entity ()
{
    int ret = 0;
    Entity * ent = createEntity();
    Signal sig = { .name = "Input signal", .length = 1, .dir = IN };

    mu_assert(ent->count_in == 0, "count_in is wrong before adding input signal.");
    mu_assert(ent->count_out == 0, "count_out is wrong before adding input signal.");
    mu_assert(ent->count_inout == 0, "count_inout is wrong before adding input signal.");

    ret = addSignalToEntity(ent, &sig);

    mu_assert(ret == 0, "Couldn't add signal to entity.");
    mu_assert(ent->count_in == 1, "count_in is wrong after adding input signal.");
    mu_assert(ent->count_out == 0, "count_out is wrong after adding input signal.");
    mu_assert(ent->count_inout == 0, "count_inout is wrong after adding input signal.");

    return NULL;
}


char * all_tests()
{
    mu_suite_start();

    mu_run_test(create_and_destroy_entity);
    mu_run_test(add_in_signal_to_entity);

    return NULL;
}

RUN_TESTS(all_tests);
