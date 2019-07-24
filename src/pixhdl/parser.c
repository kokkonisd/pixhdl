#include "parser.h"


int isLegalPortNameChar (const char portname)
{
    // Port names can only contain:
    // a-z, A-Z, 0-9 and '_'
    return isalnum(portname) || portname == '_';
}


char * parseSignalName (const char * raw_txt, int start, int end)
{
    // Name of the signal to be returned
    char * name = NULL;

    // Check if raw_txt is a valid pointer
    check(raw_txt, "Text pointer is NULL.");
    // Check if start and end are correct
    check(end > start && end > 0 && start >= 0,
          "Start and end indices are incorrect.");

    // Trim away any non-valid port name characters
    while (!isLegalPortNameChar((raw_txt + start)[0])) start++;
    while (!isLegalPortNameChar((raw_txt + end - 1)[0])) end--;

    // Check if start and end are still correct
    check(end > start && end > 0 && start >= 0,
          "Start and end indices are incorrect.");

    // Allocate memory for the name string
    name = malloc(sizeof(char) * (end - start + 1));
    check_mem(name);

    // Copy the port name string over
    strncpy(name, raw_txt + start, end - start);
    // Add a null char at the end for safety
    name[end - start] = '\0';

    // Return the port name
    return name;

error:
    return NULL;
}


direction parseSignalDirection (const char * raw_txt, int start, int end)
{
    // Resulting direction
    direction dir;
    // Temporary variable to hold the direction string
    char * dir_str = NULL;

    // Check if raw_txt is a valid pointer
    check(raw_txt, "Text pointer is NULL.");
    // Check if start and end are correct
    check(end > start && end > 0 && start >= 0,
          "Start and end indices are incorrect.");

    // Trim the spaces around the direction string
    while (isspace(*(raw_txt + start))) start++;
    while (isspace(*(raw_txt + end - 1))) end--;

    // Check if start and end are still correct
    check(end > start && end > 0 && start >= 0,
          "Start and end indices are incorrect.");

    // Allocate memory for the direction string
    dir_str = malloc(sizeof(char) * (end - start + 1));
    check_mem(dir_str);

    // Copy the direction over
    strncpy(dir_str, raw_txt + start, end - start);
    // Add a null char at the end for safety
    dir_str[end - start] = '\0';

    // Parse direction based on dir_str
    if (strcmp(dir_str, "in") == 0)
        dir = IN;
    else if (strcmp(dir_str, "out") == 0)
        dir = OUT;
    else
        dir = INOUT;

    // Free temporary string
    free(dir_str);

    // Return the direction
    return dir;

error:
    return -1;
}


int parseSignalLength (const char * raw_txt, int start, int end)
{
    // Resulting signal length (in bits)
    int length = 0;
    char * len_str = NULL;
    regex_t regex;
    int reti = 0;
    regmatch_t rm[2];
    int match_start = 0;
    int match_end = 0;

    // Check if raw_txt is a valid pointer
    check(raw_txt, "Text pointer is NULL.");
    // Check if start and end are correct
    check(end > start && end > 0 && start >= 0,
          "Start and end indices are incorrect.");

    // Verify that the length string only contains std_logic or std_logic_vector definitions
    check(*(raw_txt + start) == 's', "Invalid length string.");
    check(*(raw_txt + end - 1) == 'c' || *(raw_txt + end - 1) == ')', "Invalid length string.");

    // Allocate memory for the length string
    len_str = malloc(sizeof(char) * (end - start + 1));
    check_mem(len_str);

    // Copy the length string over
    strncpy(len_str, raw_txt + start, end - start);
    // Add a null char at the end for safety
    len_str[end - start] = '\0';

    // If it's a `std_logic_vector (X downto 0)`
    if (len_str[strlen(len_str) - 1] == ')') {
        // Compile regex to find vector length
        reti = regcomp(&regex, "([0-9]+)", REG_EXTENDED);
        check(reti == 0, "Could not compile vector length regex.");
        // Look for vector length in  the length string
        reti = regexec(&regex, len_str, 2, rm, 0);
        check(reti == 0, "Vector length not found.");

        // Get start and end of match
        match_start = rm[1].rm_so;
        match_end = rm[1].rm_eo;
        // Free the regex object, we're done using it
        regfree(&regex);

        // Allocate memory for vector length number
        char * num = malloc(sizeof(char) * (match_end - match_start));
        check_mem(num);

        // Copy the length number over
        memcpy(num, len_str + match_start, match_end - match_start);

        // Length is X + 1
        length = atoi(num) + 1;

        // Free the temporary length string
        free(num);
    // Else, the signal is a `std_logic`
    } else {
        // Thus, the length is 1 bit
        length = 1;
    }

    // Free the length string
    free(len_str);

    // Return the length of the signal
    return length;

error:
    return 0;
}


char * getRawEntityTextFromFile (const char * filename)
{
    // Pointer to the VHDL source file
    FILE * src_file = NULL;
    // Integer to contain file length
    size_t file_length = 0;
    // Integer to contain the chars that
    // were successfully read from the
    // .vhdl source file
    size_t chars_read = 0;
    // Buffer for the contents of the file
    char * buffer = NULL;
    // Regex object to parse file for entity definition
    regex_t regex;
    // Return value for the regex object
    int reti = 0;
    // Regex match object to capture matches
    regmatch_t rm[2];
    // Resulting string containing the raw entity text
    char * res = NULL;
    // Integer variables to hold the start & end of the
    // resulting string
    int start = 0;
    int end = 0;


    src_file = fopen(filename, "r");
    check(src_file != NULL, "Couldn't open file `%s`.", filename);

    // Count the length of the file (in bytes)
    fseek(src_file, 0, SEEK_END);
    file_length = ftell(src_file);
    // Reset the file pointer
    fseek(src_file, 0, SEEK_SET);

    // Allocate memory to load the entire file in a buffer
    buffer = malloc(file_length * sizeof(char));
    check_mem(buffer);

    // Read the entire file into the buffer
    chars_read = fread(buffer, 1, file_length, src_file);
    check(chars_read == file_length, "Failed to read file `%s`.", filename);

    // Close the file
    fclose(src_file);

    // Look for entity definition in the file
    reti = regcomp(&regex, ENTITY_REGEX, REG_EXTENDED);
    check(reti == 0, "Couldn't compile raw entity regex.");

    reti = regexec(&regex, buffer, 2, rm, 0);
    check(reti == 0, "Couldn't find an entity definition in file `%s`.", filename);

    // Get the start & end points from the match found
    start = rm[1].rm_so;
    end = rm[1].rm_eo;
    // Free the regex object, we don't need it anymore
    regfree(&regex);

    // Trim the match (it starts with 'entity' and ends
    // with 'end', so any char that's not a letter can
    // be chopped off)
    while (!isalpha(*(buffer + start))) start++;
    while (!isalpha(*(buffer + end - 1))) end--;

    // Check if start and end are correct
    check(end > start && end > 0 && start >= 0,
          "File `%s` doesn't contain entity, or is wrongly formatted.", filename);

    // Allocate memory for the resulting string
    res = malloc(sizeof(char) * (end - start + 2));
    check_mem(res);
    // Copy the entity definition over
    strncpy(res, buffer + start, end - start + 1);
    // End it with a semicolon to help parse ports later
    res[end - start] = ';';
    // Add a null char at the end for safety
    res[end - start + 1] = '\0';

    // Free the buffer holding the file contents
    free(buffer);

    // Return the result
    return res;

error:
    return NULL;
}


Entity * getEntityFromRawEntityText (const char * entity_text)
{
    // Regex object for parsing
    regex_t regex;
    // Return value for regex object
    int reti = 0;
    // Regex match object to capture matches
    regmatch_t rm[4];
    // Char pointer to move around entity_text
    const char * cur_text = NULL;
    // Entity object to be returned
    Entity * ent = NULL;
    // Name string to help with parsing port names
    char * name = NULL;
    // Comma index to help with parsing port names
    size_t comma_index = 0;

    // Create the Entity object
    ent = createEntity();
    check_mem(ent);
    // Start at the beginning of entity_text
    cur_text = entity_text;

    do {
        // Get entity port with regex
        reti = regcomp(&regex, PORT_REGEX, REG_EXTENDED);
        check(reti == 0, "Couldn't compile port regex.");
        reti = regexec(&regex, cur_text, 4, rm, 0);
        check(reti == 0, "Couldn't find port regex in entity.");

        // Get the name of the entity port
        name = parseSignalName(cur_text, rm[1].rm_so, rm[1].rm_eo);
        // Look for a comma character in the port name
        // (ports can be defined as `A, B, C...`)
        comma_index = strcspn(name, ",");

        do {
            // Setup a Signal object
            Signal s;

            // Allocate memory for the signal's name
            s.name = malloc(sizeof(char) * (comma_index + 1));
            // Copy the parsed name over up to comma_index
            // If only one name is defined (no commas are found),
            // the entire name gets copied over
            strncpy(s.name, name, comma_index);
            // Add a null char at the end as a safeguard
            s.name[comma_index] = '\0';

            // Parse the direction and the length of the signal
            s.dir = parseSignalDirection(cur_text, rm[2].rm_so, rm[2].rm_eo);
            s.length = parseSignalLength(cur_text, rm[3].rm_so, rm[3].rm_eo);

            // Add the signal to the entry
            addSignalToEntity(ent, &s);

            // If there are multiple signals
            if (comma_index != strlen(name)) {
                // Parse the name again, this time starting from the position of the first comma
                char * new_name = parseSignalName(name, comma_index + 1, strlen(name));
                // Free the old name
                free(name);
                // Assign the new char pointer to name
                name = new_name;
                // Look for a comma yet again
                comma_index = strcspn(name, ",");

                // If no more commas are found, this
                // is the last signal in the multiple definition
                if (comma_index == strlen(name)) {
                    // Setup the last signal as before
                    Signal s;

                    // Allocate memory for the signal's name
                    s.name = malloc(sizeof(char) * (comma_index + 1));
                    // Copy the name over
                    strncpy(s.name, name, comma_index);
                    // Add a null char safeguard at the end
                    s.name[comma_index] = '\0';

                    // Parse direction & length
                    s.dir = parseSignalDirection(cur_text, rm[2].rm_so, rm[2].rm_eo);
                    s.length = parseSignalLength(cur_text, rm[3].rm_so, rm[3].rm_eo);
                    // Add the signal to the entry
                    addSignalToEntity(ent, &s);
                }
            }
        // Repeat while there are still commas in the name string
        } while (comma_index != strlen(name));

        // Free the helper variable
        free(name);
        // Free the regex object used to parse the entityport
        regfree(&regex);

        // Check if there's another entity port to parse
        reti = regcomp(&regex, PORT_REGEX, REG_EXTENDED);
        check(reti == 0, "Couldn't compile port regex.");
        // Move the text pointer forwards
        cur_text += rm[3].rm_eo + 1;
        // Look for another entity port
        reti = regexec(&regex, cur_text, 4, rm, 0);
        // Free the regex object
        regfree(&regex);
    // Repeat whiel there are still entity ports left in the entity string
    } while (reti == 0);

    // Return entity object
    return ent;

error:
    regfree(&regex);
    if (name) free(name);
    return NULL;
}


Entity * getEntityFromFile (const char * filename)
{
    // Raw entity text string
    char * raw_entity_text = NULL;
    // Entity object to be returned
    Entity * ent = NULL;

    // Get raw entity text from given source file
    raw_entity_text = getRawEntityTextFromFile(filename);
    check(raw_entity_text, "Couldn't find entity in file `%s`.", filename);

    // Parse entity from the raw entity text
    ent = getEntityFromRawEntityText(raw_entity_text);
    check(ent, "Couldn't parse entity from file `%s`.", filename);

    // Free the raw entity text
    free(raw_entity_text);

    // Return parsed entity
    return ent;

error:
    if (raw_entity_text) free(raw_entity_text);
    return NULL;
}
