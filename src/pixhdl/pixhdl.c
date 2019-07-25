#include "pixhdl.h"


/**
 * Prints a help screen containing the available optional arguments.
 * @return (int): always returns 0
 */
int print_help_screen ()
{
    log_info(HELP_SCREEN_TEXT);

    return 0;
}


int main (int argc, char * argv[])
{
    int ra = 0;
    Entity * ent = NULL;
    int i = 0;

    // If no arguments are given, print the help screen
    if (argc == 1) return print_help_screen();

    // Parse optional arguments
    while ((ra = getopt(argc, argv, ":vh")) != -1) {
        switch (ra) {
            case 'v':
                // Print version number
                log_info("version: %s", VERSION);
                return 0;

            case 'h':
                // Print the help screen
                print_help_screen();
                return 0;

            case '?':
            default:
                // Unknown optional argument
                log_err("Unknown argument `%s`. Use `pixhdl -h` to get a complete list of valid arguments.", argv[optind - 1]);
        }
    }

    // Parse file arguments
    for (i = optind; i < argc; i++) {
        // Try to get entity from file
        ent = getEntityFromFile(argv[i]);
        check(ent, "Couldn't get entity from file `%s`.", argv[i]);

        // Print the entity's signals
        printEntitySignals(ent);

        // Destroy the entity object
        destroyEntity(ent);
    }

    return 0;

error:
    return 1;
}
