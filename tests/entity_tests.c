#include "minunit.h"
#include <pixhdl/entity.h>
#include <pixhdl/print_entity.h>


char * create_and_destroy_entity ()
{
    Entity * ent = createEntity();
    mu_assert(ent, "Entity could not be created.");

    destroyEntity(ent);

    return NULL;
}


char * add_signals_to_entity ()
{
    int ret = 0;
    Entity * ent = createEntity();
    char * name_in = "Input signal";
    char * name_out = "Output signal";
    char * name_inout = "Throughput signal";
    Signal sig_in = { .name = NULL, .length = NULL, .dir = IN };
    Signal sig_out = { .name = NULL, .length = NULL, .dir = OUT };
    Signal sig_inout = { .name = NULL, .length = NULL, .dir = INOUT };

    sig_in.name = malloc(sizeof(char) * (strlen(name_in) + 1));
    sig_out.name = malloc(sizeof(char) * (strlen(name_out) + 1));
    sig_inout.name = malloc(sizeof(char) * (strlen(name_inout) + 1));

    strncpy(sig_in.name, name_in, strlen(name_in) + 1);
    strncpy(sig_out.name, name_out, strlen(name_out) + 1);
    strncpy(sig_inout.name, name_inout, strlen(name_inout) + 1);

    sig_in.length = malloc(sizeof(char) * 2);
    sig_out.length = malloc(sizeof(char) * 2);
    sig_inout.length = malloc(sizeof(char) * 2);

    strncpy(sig_in.length, "1", 2);
    strncpy(sig_out.length, "1", 2);
    strncpy(sig_inout.length, "1", 2);

    mu_assert(ent->count_in == 0, "count_in is wrong before adding input signal.");
    mu_assert(ent->count_out == 0, "count_out is wrong before adding input signal.");
    mu_assert(ent->count_inout == 0, "count_inout is wrong before adding input signal.");

    ret = addSignalToEntity(ent, &sig_in);

    mu_assert(ret == 0, "Couldn't add signal to entity.");
    mu_assert(ent->count_in == 1, "count_in is wrong after adding input signal.");
    mu_assert(ent->count_out == 0, "count_out is wrong after adding input signal.");
    mu_assert(ent->count_inout == 0, "count_inout is wrong after adding input signal.");

    ret = addSignalToEntity(ent, &sig_out);

    mu_assert(ret == 0, "Couldn't add signal to entity.");
    mu_assert(ent->count_in == 1, "count_in is wrong after adding output signal.");
    mu_assert(ent->count_out == 1, "count_out is wrong after adding output signal.");
    mu_assert(ent->count_inout == 0, "count_inout is wrong after adding output signal.");

    ret = addSignalToEntity(ent, &sig_inout);

    mu_assert(ret == 0, "Couldn't add signal to entity.");
    mu_assert(ent->count_in == 1, "count_in is wrong after adding throughput signal.");
    mu_assert(ent->count_out == 1, "count_out is wrong after adding throughput signal.");
    mu_assert(ent->count_inout == 1, "count_inout is wrong after adding throughput signal.");

    destroyEntity(ent);

    return NULL;
}


char * get_data_from_entity_signals ()
{
    int ret = 0;
    Signal s;
    Entity * ent = createEntity();
    char * name_in = "Input signal";
    char * name_out = "Output signal";
    char * name_inout = "Throughput signal";
    Signal sig_in = { .name = NULL, .length = NULL, .dir = IN };
    Signal sig_out = { .name = NULL, .length = NULL, .dir = OUT };
    Signal sig_inout = { .name = NULL, .length = NULL, .dir = INOUT };

    sig_in.name = malloc(sizeof(char) * (strlen(name_in) + 1));
    sig_out.name = malloc(sizeof(char) * (strlen(name_out) + 1));
    sig_inout.name = malloc(sizeof(char) * (strlen(name_inout) + 1));

    strncpy(sig_in.name, name_in, strlen(name_in) + 1);
    strncpy(sig_out.name, name_out, strlen(name_out) + 1);
    strncpy(sig_inout.name, name_inout, strlen(name_inout) + 1);

    sig_in.length = malloc(sizeof(char) * 3);
    sig_out.length = malloc(sizeof(char) * 3);
    sig_inout.length = malloc(sizeof(char) * 3);

    strncpy(sig_in.length, "16", 3);
    strncpy(sig_out.length, "16", 3);
    strncpy(sig_inout.length, "16", 3);

    ret = addSignalToEntity(ent, &sig_in);
    ret = addSignalToEntity(ent, &sig_out);
    ret = addSignalToEntity(ent, &sig_inout);

    s = ent->signals_in[0];

    mu_assert(strcmp(s.name, "Input signal") == 0, "Wrong name for input signal.");
    mu_assert(s.dir == IN, "Wrong direction for input signal.");
    mu_assert(strcmp(s.length, "16") == 0, "Wrong length for input signal.");

    s = ent->signals_out[0];

    mu_assert(strcmp(s.name, "Output signal") == 0, "Wrong name for output signal.");
    mu_assert(s.dir == OUT, "Wrong direction for output signal.");
    mu_assert(strcmp(s.length, "16") == 0, "Wrong length for output signal.");

    s = ent->signals_inout[0];

    mu_assert(strcmp(s.name, "Throughput signal") == 0, "Wrong name for throughput signal.");
    mu_assert(s.dir == INOUT, "Wrong direction for throughput signal.");
    mu_assert(strcmp(s.length, "16") == 0, "Wrong length for throughput signal.");

    destroyEntity(ent);

    return NULL;
}


char * convert_directions_to_strings ()
{
    mu_assert(strcmp(directionToString(IN), "IN") == 0, "Wrong string for direction IN.");
    mu_assert(strcmp(directionToString(OUT), "OUT") == 0, "Wrong string for direction OUT.");
    mu_assert(strcmp(directionToString(INOUT), "INOUT") == 0, "Wrong string for direction INOUT.");

    return NULL;
}


char * all_tests ()
{
    mu_suite_start();

    mu_run_test(create_and_destroy_entity);
    mu_run_test(add_signals_to_entity);
    mu_run_test(get_data_from_entity_signals);
    mu_run_test(convert_directions_to_strings);

    return NULL;
}

RUN_TESTS(all_tests);
