#include "minunit.h"
#include <pixhdl/generate_svg.h>
#include <unistd.h>


char * call_generate_with_invalid_entity ()
{
    int rv = generateSvgFromEntity(NULL, "test.svg");
    mu_assert(rv == 1, "Generation didn't fail with invalid entity.");

    return NULL;
}

char * call_generate_with_invalid_filename ()
{
    Entity * ent = createEntity();

    int rv = generateSvgFromEntity(ent, NULL);
    mu_assert(rv == 1, "Generation didn't fail with invalid filename.");

    destroyEntity(ent);

    return NULL;
}

char * call_generate_with_invalid_entity_and_invalid_filename ()
{
    int rv = generateSvgFromEntity(NULL, NULL);
    mu_assert(rv == 1, "Generation didn't fail with invalid entity and invalid filename.");

    return NULL;
}

char * call_generate_with_valid_entity_and_filename ()
{
    Entity * ent = getEntityFromFile("tests/vhdl_sources/INOUT.vhdl", NULL);

    int rv = generateSvgFromEntity(ent, "/tmp/pixhdl_test.svg");
    mu_assert(rv == 0, "Generation failed with valid entity and valid filename.");

    mu_assert(access("/tmp/pixhdl_test.svg", F_OK) != -1, "SVG file wasn't created.");

    destroyEntity(ent);

    return NULL;
}

char * all_tests ()
{
    mu_suite_start();

    mu_run_test(call_generate_with_invalid_entity);
    mu_run_test(call_generate_with_invalid_filename);
    mu_run_test(call_generate_with_invalid_entity_and_invalid_filename);
    mu_run_test(call_generate_with_valid_entity_and_filename);

    return NULL;
}

RUN_TESTS(all_tests);
