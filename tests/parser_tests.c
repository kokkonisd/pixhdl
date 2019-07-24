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


char * all_tests ()
{
    mu_suite_start();

    mu_run_test(test_legal_port_name_characters);
    mu_run_test(parse_simple_port_names);
    mu_run_test(parse_simple_port_directions);

    return NULL;
}

RUN_TESTS(all_tests);
