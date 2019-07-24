#include "minunit.h"
#include <pixhdl/parser.h>


char * test_legal_port_name_characters ()
{
    mu_assert(isLegalPortNameChar('A'),
              "Uppercase letters are legal port name characters.");

    mu_assert(isLegalPortNameChar('z'),
              "Lowercase letters are legal port name characters.");

    mu_assert(isLegalPortNameChar('5'),
              "Digits are legal port name characters.");

    mu_assert(isLegalPortNameChar('_'),
              "Underscores are legal port name characters.");

    mu_assert(!isLegalPortNameChar(' '),
              "Spaces are illegal port name characters.");

    mu_assert(!isLegalPortNameChar('+'),
              "Special characters are illegal port name characters.");

    return NULL;
}


char * parse_simple_port_names ()
{
    char * name;

    name = parseSignalName("     A      ", 0, 12);
    mu_assert(strcmp(name, "A") == 0, "Incorrect parsing of port name 'A'.");
    free(name);

    name = parseSignalName("B, C", 0, 4);
    mu_assert(strcmp(name, "B, C") == 0, "Incorrect parsing of port name 'B, C'.");
    free(name);

    name = parseSignalName("D : in std_logic", 0, 4);
    mu_assert(strcmp(name, "D") == 0, "Incorrect parsing of port name 'D'.");
    free(name);

    name = parseSignalName("E:in std_logic_vector(3 downto 0);", 0, 2);
    mu_assert(strcmp(name, "E") == 0, "Incorrect parsing of port name 'E'.");
    free(name);

    return NULL;
}


char * parse_simple_port_directions ()
{
    direction dir;

    dir = parseSignalDirection("     in      ", 0, 12);
    mu_assert(dir == IN, "Incorrect parsing of port dir 'IN'.");

    dir = parseSignalDirection(": out", 0, 5);
    mu_assert(dir == OUT, "Incorrect parsing of port dir 'OUT'.");

    dir = parseSignalDirection("D : inout std_logic", 1, 9);
    mu_assert(dir == INOUT, "Incorrect parsing of port dir 'INOUT'.");

    dir = parseSignalDirection("E:in std_logic_vector(3 downto 0);", 1, 4);
    mu_assert(dir == IN, "Incorrect parsing of port dir 'IN' (2).");

    return NULL;
}


char * parse_simple_port_lengths ()
{
    int length = 0;

    length = parseSignalLength("std_logic", 0, 9);
    mu_assert(length == 1, "Incorrect parsing of port length 1.");

    length = parseSignalLength(": std_logic_vector(10 downto 0)", 2, 31);
    mu_assert(length == 11, "Incorrect parsing of port length 11.");

    length = parseSignalLength("D : inout std_logic", 10, 19);
    mu_assert(length == 1, "Incorrect parsing of port length 1 (2).");

    length = parseSignalLength("E:in std_logic_vector(3 downto 0);", 5, 33);
    mu_assert(length == 4, "Incorrect parsing of port length 4.");

    return NULL;
}


char * parse_raw_text_entity_from_file ()
{
    char * right_entity_text = NULL;
    char * parsed_entity_text = NULL;
    char * filename = NULL;

    filename = "tests/vhdl_sources/ALU.vhdl";
    right_entity_text = "OP : in std_logic_vector (1 downto 0);\n\
        A, B, C, D, E, F, G : in std_logic_vector (31 downto 0);\n\
        O : out std_logic_vector (31 downto 0);\n\
        N : out std_logic;";

    parsed_entity_text = getRawEntityTextFromFile(filename);
    mu_assert(strcmp(parsed_entity_text, right_entity_text) == 0,
              "Raw entity text is wrong for the first file.");
    free(parsed_entity_text);


    filename = "tests/vhdl_sources/ALU_oneline.vhdl";
    right_entity_text = "OP:in std_logic_vector (1 downto 0);A,B,C,D,E,F,G:in std_logic_vector (31 downto 0);O:out std_logic_vector (31 downto 0);N:out std_logic;";

    parsed_entity_text = getRawEntityTextFromFile(filename);
    mu_assert(strcmp(parsed_entity_text, right_entity_text) == 0,
              "Raw entity text is wrong for the second file.");
    free(parsed_entity_text);

    return NULL;

}


char * all_tests ()
{
    mu_suite_start();

    mu_run_test(test_legal_port_name_characters);
    mu_run_test(parse_simple_port_names);
    mu_run_test(parse_simple_port_directions);
    mu_run_test(parse_simple_port_lengths);
    mu_run_test(parse_raw_text_entity_from_file);

    return NULL;
}

RUN_TESTS(all_tests);
