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


char * all_tests()
{
    mu_suite_start();

    mu_run_test(create_and_destroy_entity);

    return NULL;
}

RUN_TESTS(all_tests);
