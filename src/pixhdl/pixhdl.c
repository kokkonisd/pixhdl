#include "pixhdl.h"


int print_help_screen ()
{
    log_info(HELP_SCREEN_TEXT);

    return 0;
}


int main (int argc, char * argv[])
{
    // Character value for argument parsing
    int c = 0;
    // Option index for long arguments
    int option_index = 0;
    // Input filename
    char * in_filename = NULL;
    // Output filename
    char * out_filename = NULL;
    // Clock signal name
    char * clock_name = NULL;
    // Print entity flag
    int print_entity = 0;
    // Return value for the SVG generator
    int rgen = 0;
    // Entity resulting from file parsing
    Entity * ent = NULL;
    // Long argument structure
    static struct option long_options[] = {
        { "version",    no_argument,       0, 'v' },
        { "help",       no_argument,       0, 'h' },
        { "output",     required_argument, 0, 'o' },
        { "update",     no_argument,       0, 'u' },
        { "print",      no_argument,       0, 'p' },
        { "clock-name", required_argument, 0, 'c' },
        { 0,            0,                 0, 0   }
    };

    // If no arguments are specified, show the help screen
    if (argc == 1) return print_help_screen();

    while (1) {
        // Initialize long option index at 0
        option_index = 0;

        // Try to parse next argument
        c = getopt_long(argc, argv, ":vho:upc:", long_options, &option_index);

        // If no arguments are found, parsing is over
        if (c == -1) break;

        // Parse arguments based on the value of c
        switch (c) {
            case 'v':
                // Print version number
                log_info("Version: %s", VERSION);
                return 0;

            case 'h':
                // Print the help screen
                print_help_screen();
                return 0;

            case 'o':
                // Capture output filename
                out_filename = optarg;
                break;

            case 'u':
                // Try to update
                return update(VERSION);

            case 'p':
                // Set the print_entity flag to 1 to print entity in terminal
                print_entity = 1;
                break;

            case 'c':
                // Set the clock name
                clock_name = optarg;
                break;

            case '?':
                // Unknown argument handling
                log_err("Unknown argument `%s`. Use `pixhdl -h` to get a "\
                        "complete list of valid arguments.",
                        optind > 1 ? argv[optind - 1] : argv[1]);
                return 1;

            case ':':
                // Missing argument handling
                log_err("Option `-%c/--%s` requires an argument.",
                        (char) optopt, long_options[optind].name);
                return 1;

            default:
                // Error handling
                log_err("Couldn't parse arguments. Use `pixhdl -h` to get a "\
                        "complete list of valid arguments.");
                return 1;
        }
    }

    // If no arguments are left, the user hasn't specified any .vhd/.vhdl
    // source files
    check(optind < argc, "No .vhd/.vhdl sources specified.");

    // Capture input filename and move optind forward
    in_filename = argv[optind];
    optind++;

    // Get the entity and check that it has been parsed correctly
    ent = getEntityFromFile(in_filename, clock_name);
    check(ent, "Couldn't get entity from file `%s`.", in_filename);

    // Check that at least one of the following is true:
    //     1. There is still at least one argument to be parsed (which will be
    //        the output)
    //     2. The output filename has already been specified
    //     3. The print_entity flag has been set to 1 (so the user doesn't have
    //        to specify an output file)
    check(optind < argc || out_filename || print_entity,
          "No output SVG file specified.");

    // If the print_entity flag is set to 1, print the entity
    if (print_entity) printEntitySignals(ent);

    // If the output file is set or if there are any arguments left
    if (optind < argc || out_filename) {
        // The output file has already been set, or if not then the next
        // argument is the output file
        if (!out_filename) out_filename = argv[optind];
        // Generate SVG and check that the result is correct
        rgen = generateSvgFromEntity(ent, out_filename);
        check(rgen == 0, "Couldn't generate SVG image for file `%s`.",
                         in_filename);
    }

    // Destroy the entity object
    destroyEntity(ent);

    return 0;

error:
    // If the entity object hasn't been freed yet, destroy it
    if (ent) destroyEntity(ent);
    return 1;
}
