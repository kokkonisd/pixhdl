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
    // Return value for optional argument parsing
    int ra = 0;
    // Return value for SVG generation
    int rgen = 0;
    // Entity object to hold parsed entity
    Entity * ent = NULL;
    //
    int i = 0;
    char * out_filename = NULL;
    char * copy_ptr = NULL;
    int is_filename_allocated = 0;

    // If no arguments are given, print the help screen
    if (argc == 1) return print_help_screen();

    // Parse optional arguments
    while ((ra = getopt(argc, argv, ":vho:u")) != -1) {
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

            case 'u':
                return update(VERSION);

            case '?':
            default:
                // Show error for -o flag without an argument
                check(optopt != 'o', "Option '-o' requires an argument.");

                // Unknown optional argument
                log_err("Unknown argument `%s`. Use `pixhdl -h` to get a complete list of valid arguments.", argv[optind - 1]);
                return 1;
        }
    }

    // Check that there's still at least one argument left
    check(optind < argc, "No .vhd/.vhdl sources specified.");

    // Move optind forward if possible to check for -o flag
    if (optind + 1 < argc) optind++;
    // Check for post-input -o flag
    if (argv[optind][0] == '-') {
        // Check if it's an -o flag
        if (argv[optind][1] == 'o') {
            // Check that the flag is followed by an argument
            check(optind + 1 < argc, "Option '-o' requires an argument.");
            // Get the argument (filename)
            out_filename = argv[optind + 1];
            // Reset optind for source parsing
            optind--;
        } else {
            // Other flags are illegal when they come after the VHDL sources
            log_err("Unknown/unsupported post-input argument `%s`. Use `pixhdl -h` to get a complete list of valid arguments.", argv[optind]);
            return 1;
        }
    }

    // Try to get entity from file
    ent = getEntityFromFile(argv[optind]);
    check(ent, "Couldn't get entity from file `%s`.", argv[optind]);

    // If the filename hasn't been specified, create it
    // by just adding .svg to the end of the input file path
    if (!out_filename) {
        is_filename_allocated = 1;
        // Allocate memory for the name of the output file
        out_filename = malloc(sizeof(char) * (strlen(argv[optind]) + 5));

        // Point the copy pointer at the start of the input file string
        copy_ptr = argv[optind];
        // Initialize the char counter at 0
        i = 0;

        // Copy the whole filename except the .* ending
        while (*copy_ptr != '.')
            out_filename[i++] = *copy_ptr++;

        // Write '.svg' at the end of the filename
        out_filename[i] = '.';
        out_filename[i + 1] = 's';
        out_filename[i + 2] = 'v';
        out_filename[i + 3] = 'g';
        out_filename[i + 4] = '\0';

        // Generate an SVG
        rgen = generateSvgFromEntity(ent, out_filename);
        check(rgen == 0, "Couldn't generate SVG image for file `%s`.", argv[optind]);

        // Free the generated name
        free(out_filename);
    } else {
        // Generate an SVG
        rgen = generateSvgFromEntity(ent, out_filename);
        check(rgen == 0, "Couldn't generate SVG image for file `%s`.", argv[optind]);
    }

    // Destroy the entity object
    destroyEntity(ent);

    return 0;

error:
    if (is_filename_allocated) free(out_filename);
    return 1;
}
