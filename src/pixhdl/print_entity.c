#include "print_entity.h"


char * directionToString (direction dir)
{
    switch (dir) {
        case IN:
            return "IN";
        case OUT:
            return "OUT";
        case INOUT:
            return "INOUT";
        default:
            return "UNEFINED";
    }
}


void printEntitySignals (Entity * ent)
{
    unsigned int i = 0;
    unsigned int spaces = ent->max_name_size > 4 ? ent->max_name_size : 4;

    printf("Signal # | Name ");
    for (i = 0; i < spaces - 4; i++)
        printf(" ");

    printf("| Length (bits) | Direction\n");

    for (i = 0; i < spaces + 40; i++)
        printf("=");
    printf("\n");

    for (i = 0; i < ent->count_in; i++)
        printf("%-8d | %-*s | %-13d | %s\n", i + 1,
                                             spaces,
                                             ent->signals_in[i].name,
                                             ent->signals_in[i].length,
                                             directionToString(ent->signals_in[i].dir));

    for (i = 0; i < spaces + 40; i++)
        printf("-");
    printf("\n");

    for (i = 0; i < ent->count_out; i++)
        printf("%-8d | %-*s | %-13d | %s\n", i + 1,
                                             spaces,
                                             ent->signals_out[i].name,
                                             ent->signals_out[i].length,
                                             directionToString(ent->signals_out[i].dir));

    for (i = 0; i < spaces + 40; i++)
        printf("-");
    printf("\n");

    for (i = 0; i < ent->count_inout; i++)
        printf("%-8d | %-*s | %-13d | %s\n", i + 1,
                                             spaces,
                                             ent->signals_inout[i].name,
                                             ent->signals_inout[i].length,
                                             directionToString(ent->signals_inout[i].dir));
}
