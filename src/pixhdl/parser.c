#include "parser.h"


/**
 * Check if a given character is a legal port name in VHDL.
 * @param  portname_char (const char): A portname character
 * @return                      (int): 1 if the char is valid, 0 otherwise
 */
int isLegalPortNameChar (const char portname_char)
{
    // Port names can only contain:
    // a-z, A-Z, 0-9 and '_'
    return isalnum(portname_char) || portname_char == '_';
}


/**
 * Parses the name of a signal given some raw entity text, a start and an end index.
 * @param  raw_txt (const char *): The raw entity text
 * @param  start            (int): The beginning index for the raw entity text
 * @param  end              (int): The ending index for the raw entity text
 * @return               (char *): The resulting signal name
 */
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


/**
 * Parses the direction of a signal given some raw entity text, a start and an end index.
 * @param  raw_txt (const char *): The raw entity text
 * @param  start            (int): The beginning index for the raw entity text
 * @param  end              (int): The ending index for the raw entity text
 * @return       (direction enum): The resulting direction
 */
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
    while (!isalpha(*(raw_txt + start))) start++;
    while (!isalpha(*(raw_txt + end - 1))) end--;

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
    if (strcmp(dir_str, "in") == 0 || strcmp(dir_str, "IN") == 0)
        dir = IN;
    else if (strcmp(dir_str, "out") == 0 || strcmp(dir_str, "OUT") == 0)
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


/**
 * Parses the length of a signal (in bits) given some raw entity text, a start and an end index.
 * @param  raw_txt (const char *): The raw entity text
 * @param  start            (int): The beginning index for the raw entity text
 * @param  end              (int): The ending index for the raw entity text
 * @return                  (int): The resulting length (in bits)
 */
int parseSignalLength (const char * raw_txt, int start, int end)
{
    // Resulting signal length (in bits)
    int length = 0;
    char * len_str = NULL;
    regex_t regex;
    int reti = 0;
    regmatch_t rm[3];
    int top_match_start = 0;
    int top_match_end = 0;
    int bottom_match_start = 0;
    int bottom_match_end = 0;

    // Check if raw_txt is a valid pointer
    check(raw_txt, "Text pointer is NULL.");
    // Check if start and end are correct
    check(end > start && end > 0 && start >= 0,
          "Start and end indices are incorrect.");

    // Verify that the length string only contains std_logic or std_logic_vector definitions
    check(tolower(*(raw_txt + start)) == 's', "Invalid length string.");
    check(tolower(*(raw_txt + end - 1)) == 'c' || *(raw_txt + end - 1) == ')', "Invalid length string.");

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
        reti = regcomp(&regex, VECTOR_LENGTH_REGEX, REG_EXTENDED);
        check(reti == 0, "Could not compile vector length regex.");
        // Look for vector length in  the length string
        reti = regexec(&regex, len_str, 3, rm, 0);
        check(reti == 0, "Vector length not found.");

        // Get start and end of top (MSB) match
        top_match_start = rm[1].rm_so;
        top_match_end = rm[1].rm_eo;
        // Get start and end of bottom (LSB) match
        bottom_match_start = rm[2].rm_so;
        bottom_match_end = rm[2].rm_eo;
        // Free the regex object, we're done using it
        regfree(&regex);

        // Allocate memory for vector length number
        char * top_num = malloc(sizeof(char) * (top_match_end - top_match_start + 1));
        check_mem(top_num);
        char * bottom_num = malloc(sizeof(char) * (bottom_match_end - bottom_match_start + 1));
        check_mem(bottom_num);

        // Copy the length top_number over
        strncpy(top_num, len_str + top_match_start, top_match_end - top_match_start);
        top_num[top_match_end - top_match_start] = '\0';
        strncpy(bottom_num, len_str + bottom_match_start, bottom_match_end - bottom_match_start);
        bottom_num[bottom_match_end - bottom_match_start] = '\0';

        // Length is MSB - LSB + 1
        length = atoi(top_num) - atoi(bottom_num) + 1;

        // Free the temporary length string
        free(top_num);
        free(bottom_num);
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


/**
 * Gets the raw entry definition from a given file. Output also contains the entity's name.
 * @param  filename (const char *): The path/name of the source file
 * @return                (char *): The raw name and body of the entity definition ("name;body")
 */
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
    regmatch_t rm[3];
    // String containing the entity_name text
    char * entity_name = NULL;
    // String containing the entity_body text
    char * entity_body = NULL;
    // Resulting string containing the raw entity text
    char * res;
    // Integer variables to hold the start & end of the
    // resulting strings
    int body_start = 0;
    int body_end = 0;
    int name_start = 0;
    int name_end = 0;


    // Try to open the source file passed to the function
    src_file = fopen(filename, "r");
    check(src_file != NULL, "Couldn't open file `%s`.", filename);

    // Count the length of the file (in bytes)
    fseek(src_file, 0, SEEK_END);
    file_length = ftell(src_file);
    // Reset the file pointer
    fseek(src_file, 0, SEEK_SET);

    // Allocate memory to load the entire file in a buffer
    buffer = malloc((file_length + 1) * sizeof(char));
    check_mem(buffer);

    // Read the entire file into the buffer
    chars_read = fread(buffer, 1, file_length, src_file);
    check(chars_read == file_length, "Failed to read file `%s`.", filename);
    buffer[file_length] = '\0';

    // Close the file
    fclose(src_file);

    // Look for the entity definition in the file
    reti = regcomp(&regex, ENTITY_REGEX, REG_EXTENDED | REG_ICASE);
    check(reti == 0, "Couldn't compile raw entity regex.");

    reti = regexec(&regex, buffer, 3, rm, 0);
    check(reti == 0, "Couldn't find an entity definition in file `%s`.", filename);

    // Get the name_start & name_end points from the match found
    name_start = rm[1].rm_so;
    name_end = rm[1].rm_eo;
    // Get the body_start & body_end points from the match found
    body_start = rm[2].rm_so;
    body_end = rm[2].rm_eo;
    // Free the regex object, we don't need it anymore
    regfree(&regex);


    // Trim the name match (there's probably spaces around it)
    while (!isLegalPortNameChar(*(buffer + name_start))) name_start++;
    while (!isLegalPortNameChar(*(buffer + name_end - 1))) name_end--;
    // Trim thebody  match (it starts with 'entity' and ends
    // with 'end', so any char that's not a letter can
    // be chopped off)
    while (!isalnum(*(buffer + body_start))) body_start++;
    while (isspace(*(buffer + body_end - 1))) body_end--;

    // Check if name_start and name_end are correct
    check(name_end > name_start && name_end > 0 && name_start >= 0,
          "File `%s` doesn't contain entity, or is wrongly formatted.", filename);
    // Check if body_start and body_end are correct
    check(body_end > body_start && body_end > 0 && body_start >= 0,
          "File `%s` doesn't contain entity, or is wrongly formatted.", filename);

    // Allocate memory for the name string
    entity_name = malloc(sizeof(char) * (name_end - name_start + 2));
    check_mem(entity_name);
    // Allocate memory for the body string
    entity_body = malloc(sizeof(char) * (body_end - body_start + 2));
    check_mem(entity_body);
    // Copy the entity name over
    strncpy(entity_name, buffer + name_start, name_end - name_start + 1);
    // Copy the entity definition over
    strncpy(entity_body, buffer + body_start, body_end - body_start + 1);
    // End name and body with a semicolon to help parsing later
    entity_name[name_end - name_start] = ';';
    entity_body[body_end - body_start] = ';';
    // Add a null char at the end of both of them for safety
    entity_name[name_end - name_start + 1] = '\0';
    entity_body[body_end - body_start + 1] = '\0';

    // Free the buffer holding the file contents
    free(buffer);

    // Allocate memory for the final string
    res = malloc(sizeof(char) * (strlen(entity_name) + strlen(entity_body) + 1));
    // Print the name followed by the body in the resulting string
    sprintf(res, "%s%s", entity_name, entity_body);
    // Add a null char at the end for safety
    res[strlen(entity_name) + strlen(entity_body)] = '\0';

    // Free the name & body strings
    free(entity_name);
    free(entity_body);

    // Return the result
    return res;

error:
    return NULL;
}


/**
 * Generate an Entity object given an entity definition in raw text.
 * @param  entity_text (const char *): The raw entity definition (formatted as "name;body")
 * @return                 (Entity *): The Entity object
 */
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
    // Pointer to the name delimiter
    char * name_delimiter = NULL;
    // Integer to hold the length of the entity's name
    int name_len = 0;

    // Create the Entity object
    ent = createEntity();
    check_mem(ent);
    // Start at the beginning of entity_text
    cur_text = entity_text;

    // Get the position of the entity's name delimiter (a semicolon)
    name_delimiter = strstr(cur_text, ";");
    // Calculate the length of the entity's name
    name_len = name_delimiter - cur_text;
    // Allocate some memory for the entity's name
    ent->name = malloc(sizeof(char) * (name_len + 1));
    // Copy the name over
    strncpy(ent->name, cur_text, name_len);
    // Add a null char at the end for safety
    ent->name[name_len] = '\0';

    // Move the cur_text pointer forwards to skip the name
    cur_text += name_len + 1;

    do {
        // Get entity port with regex
        reti = regcomp(&regex, PORT_REGEX, REG_EXTENDED | REG_ICASE);
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
        reti = regcomp(&regex, PORT_REGEX, REG_EXTENDED | REG_ICASE);
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


/**
 * Generates an Entity object given a filename.
 * @param  filename (const char *): The path/name of the source file
 * @return              (Entity *): The Entity object
 */
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
