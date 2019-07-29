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
    char * out_filename = NULL;
    char * copy_ptr = NULL;
    int j = 0;

    // If no arguments are given, print the help screen
    if (argc == 1) return print_help_screen();

    // Parse optional arguments
    while ((ra = getopt(argc, argv, ":vho:")) != -1) {
        switch (ra) {
            case 'v':
                // Print version number
                log_info("version: %s", VERSION);
                return 0;

            case 'h':
                // Print the help screen
                print_help_screen();
                return 0;

            case 'o':
                out_filename = optarg;
                break;

            case '?':
            default:
                // Show error for -o flag without an argument
                if (optopt == 'o') {
                    log_err("Option '-o' requires an argument.");
                    return 1;
                }
                
                // Unknown optional argument
                log_err("Unknown argument `%s`. Use `pixhdl -h` to get a complete list of valid arguments.", argv[optind - 1]);
                return 1;
        }
    }

    // Parse file arguments
    for (i = optind; i < argc; i++) {
        // Try to get entity from file
        ent = getEntityFromFile(argv[i]);
        check(ent, "Couldn't get entity from file `%s`.", argv[i]);

        if (!out_filename) {
            // Allocate memory for the name of the output file
            out_filename = malloc(sizeof(char) * (strlen(argv[i]) + 5));

            // Point the copy pointer at the start of the input file string
            copy_ptr = argv[i];
            // Initialize the char counter at 0
            j = 0;

            // Copy the whole filename except the .* ending
            while (*copy_ptr != '.')
                out_filename[j++] = *copy_ptr++;

            // Write '.svg' at the end of the filename
            out_filename[j] = '.';
            out_filename[j + 1] = 's';
            out_filename[j + 2] = 'v';
            out_filename[j + 3] = 'g';
            out_filename[j + 4] = '\0';

            // Generate an SVG
            generateSvgFromEntity(ent, out_filename);

            // Free the generated name
            free(out_filename);
        } else {
            // Generate an SVG
            generateSvgFromEntity(ent, out_filename);
        }

        // Destroy the entity object
        destroyEntity(ent);
    }

    return 0;

error:
    return 1;
}
