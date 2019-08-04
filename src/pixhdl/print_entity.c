#include "print_entity.h"


const char * directionToString (direction dir)
{
    switch (dir) {
        case IN:
            return "IN";
        case OUT:
            return "OUT";
        case INOUT:
            return "INOUT";
        case GENERIC:
            return "GENERIC";
        default:
            return "UNDEFINED";
    }
}


void printEntitySignals (Entity * ent)
{
    // For-loop counter
    unsigned int i = 0;
    // Counter for the amount of spaces a name can occupy
    unsigned int name_spaces = ent->max_name_size_global > 4 ? ent->max_name_size_global : 4;
    unsigned int length_spaces = ent->max_length_size_global > 32 ? ent->max_length_size_global : 32;

    for (i = 0; i < name_spaces + length_spaces + 27; i++)
        printf("-");
    printf("\n");

    printf("| Entity %-*s |\n", name_spaces + length_spaces + 16, ent->name);

    for (i = 0; i < name_spaces + length_spaces + 27; i++)
        printf("-");
    printf("\n");

    // Print the table header
    printf("Signal # | Name ");
    for (i = 0; i < name_spaces - 4; i++)
        printf(" ");

    printf("| Value (generics) / Length (bits) ");
    for (i = 0; i < length_spaces - 32; i++)
        printf(" ");

    printf("| Direction\n");

    for (i = 0; i < name_spaces + length_spaces + 27; i++)
        printf("=");
    printf("\n");


    // Print the GENERIC inputs
    for (i = 0; i < ent->count_generics; i++)
        printf("%-8d | %-*s | %-*s | %s\n", i + 1,
                                             name_spaces,
                                             ent->generics[i].name,
                                             length_spaces,
                                             ent->generics[i].length,
                                             directionToString(ent->generics[i].dir));

    for (i = 0; i < name_spaces + length_spaces + 27; i++)
        printf("-");
    printf("\n");


    // Print the IN signals
    for (i = 0; i < ent->count_in; i++)
        printf("%-8d | %-*s | %-*s | %s\n", i + 1,
                                             name_spaces,
                                             ent->signals_in[i].name,
                                             length_spaces,
                                             ent->signals_in[i].length,
                                             directionToString(ent->signals_in[i].dir));

    for (i = 0; i < name_spaces + length_spaces + 27; i++)
        printf("-");
    printf("\n");


    // Print the OUT signals
    for (i = 0; i < ent->count_out; i++)
        printf("%-8d | %-*s | %-*s | %s\n", i + 1,
                                             name_spaces,
                                             ent->signals_out[i].name,
                                             length_spaces,
                                             ent->signals_out[i].length,
                                             directionToString(ent->signals_out[i].dir));

    for (i = 0; i < name_spaces + length_spaces + 27; i++)
        printf("-");
    printf("\n");


    // Print the INOUT signals
    for (i = 0; i < ent->count_inout; i++)
        printf("%-8d | %-*s | %-*s | %s\n", i + 1,
                                             name_spaces,
                                             ent->signals_inout[i].name,
                                             length_spaces,
                                             ent->signals_inout[i].length,
                                             directionToString(ent->signals_inout[i].dir));
}
