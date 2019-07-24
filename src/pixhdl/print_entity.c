#include "print_entity.h"


/**
 * Convert a direction enum to string.
 * @param  dir (direction enum): The direction to be converted
 * @return       (const char *): The string equivalent of the direction
 */
const char * directionToString (direction dir)
{
    switch (dir) {
        case IN:
            return "IN";
        case OUT:
            return "OUT";
        case INOUT:
            return "INOUT";
        default:
            return "UNDEFINED";
    }
}


/**
 * Prints all the signals of an Entity in the following order: IN, OUT, INOUT.
 * @param ent (Entity *): The Entity object whose signals are to be printed
 */
void printEntitySignals (Entity * ent)
{
    // For-loop counter
    unsigned int i = 0;
    // Counter for the amount of spaces a name can occupy
    unsigned int spaces = ent->max_name_size > 4 ? ent->max_name_size : 4;

    for (i = 0; i < spaces + 40; i++)
        printf("-");
    printf("\n");

    printf("| Entity %-*s |\n", spaces + 29, ent->name);

    for (i = 0; i < spaces + 40; i++)
        printf("-");
    printf("\n");

    // Print the table header
    printf("Signal # | Name ");
    for (i = 0; i < spaces - 4; i++)
        printf(" ");

    printf("| Length (bits) | Direction\n");

    for (i = 0; i < spaces + 40; i++)
        printf("=");
    printf("\n");


    // Print the IN signals
    for (i = 0; i < ent->count_in; i++)
        printf("%-8d | %-*s | %-13d | %s\n", i + 1,
                                             spaces,
                                             ent->signals_in[i].name,
                                             ent->signals_in[i].length,
                                             directionToString(ent->signals_in[i].dir));

    for (i = 0; i < spaces + 40; i++)
        printf("-");
    printf("\n");


    // Print the OUT signals
    for (i = 0; i < ent->count_out; i++)
        printf("%-8d | %-*s | %-13d | %s\n", i + 1,
                                             spaces,
                                             ent->signals_out[i].name,
                                             ent->signals_out[i].length,
                                             directionToString(ent->signals_out[i].dir));

    for (i = 0; i < spaces + 40; i++)
        printf("-");
    printf("\n");


    // Print the INOUT signals
    for (i = 0; i < ent->count_inout; i++)
        printf("%-8d | %-*s | %-13d | %s\n", i + 1,
                                             spaces,
                                             ent->signals_inout[i].name,
                                             ent->signals_inout[i].length,
                                             directionToString(ent->signals_inout[i].dir));
}
