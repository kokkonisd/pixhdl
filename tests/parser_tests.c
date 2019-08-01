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


char * test_alphabetic_length_strings ()
{
    mu_assert(containsAlpha("N - 1"), "N is an alphabetic character.");
    mu_assert(containsAlpha("M"), "M is an alphabetic character.");
    mu_assert(!containsAlpha("15"), "Neither 1 nor 5 are alphabetic characters.");
    mu_assert(!containsAlpha("1 - 5"), "Neither 1 nor - nor 5 are alphabetic characters.");

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

    dir = parseSignalDirection("D : INOUT std_logic", 1, 9);
    mu_assert(dir == INOUT, "Incorrect parsing of port dir 'INOUT'.");

    dir = parseSignalDirection("E:in std_logic_vector(3 downto 0);", 1, 4);
    mu_assert(dir == IN, "Incorrect parsing of port dir 'IN' (2).");

    return NULL;
}


char * parse_simple_port_lengths ()
{
    char * length = NULL;

    length = parseSignalLength("std_logic", 0, 9);
    mu_assert(strcmp(length, "1") == 0, "Incorrect parsing of port length 1.");
    free(length);

    length = parseSignalLength(": std_logic_vector(10 downto 0)", 2, 31);
    mu_assert(strcmp(length, "11") == 0, "Incorrect parsing of port length 11.");
    free(length);

    length = parseSignalLength("D : inout std_logic", 10, 19);
    mu_assert(strcmp(length, "1") == 0, "Incorrect parsing of port length 1 (2).");
    free(length);

    length = parseSignalLength("E:in std_logic_vector(3 downto 1);", 5, 33);
    mu_assert(strcmp(length, "3") == 0, "Incorrect parsing of port length 3.");
    free(length);

    length = parseSignalLength("F:in std_logic_vector(N - 1 downto 0);", 5, 37);
    mu_assert(strcmp(length, "(N - 1 downto 0)") == 0, "Incorrect parsing of port length N.");
    free(length);

    length = parseSignalLength("F:in std_logic_vector(N downto M);", 5, 33);
    mu_assert(strcmp(length, "(N downto M)") == 0, "Incorrect parsing of port length N - M + 1.");
    free(length);

    return NULL;
}


char * parse_raw_text_entity_from_file ()
{
    char * right_entity_text = NULL;
    char * parsed_entity_text = NULL;
    char * filename = NULL;

    filename = "tests/vhdl_sources/ALU.vhdl";
    right_entity_text = "ALU;@OP : in std_logic_vector (1 downto 0);\n\
        A, B, C, D, E, F, G : in std_logic_vector (31 downto 0);\n\
        O : out std_logic_vector (31 downto 0);\n\
        N : out std_logic;";

    parsed_entity_text = getRawEntityTextFromFile(filename);
    mu_assert(strcmp(parsed_entity_text, right_entity_text) == 0,
              "Raw entity text is wrong for the first file.");
    free(parsed_entity_text);


    filename = "tests/vhdl_sources/ALU_oneline.vhdl";
    right_entity_text = "ALU;@OP:in std_logic_vector (1 downto 0);A,B,C,D,E,F,G:in std_logic_vector (31 downto 0);O:out std_logic_vector (31 downto 0);N:out std_logic;";

    parsed_entity_text = getRawEntityTextFromFile(filename);
    mu_assert(strcmp(parsed_entity_text, right_entity_text) == 0,
              "Raw entity text is wrong for the second file.");
    free(parsed_entity_text);


    filename = "tests/vhdl_sources/MUX_generic.vhdl";
    right_entity_text = "MUX;*N : integer range 0 to 32;\n\
        M : time 0.2 ns;@A, B : in std_logic_vector (N - 1 downto 0);\n\
        COM : in std_logic;\n\
        Y : out std_logic_vector (N - 1 downto 0);";

    parsed_entity_text = getRawEntityTextFromFile(filename);
    mu_assert(strcmp(parsed_entity_text, right_entity_text) == 0,
              "Raw entity text is wrong for the third file.");
    free(parsed_entity_text);

    return NULL;
}


char * parse_entity_from_raw_entity_text ()
{
    char * raw_entity_text = NULL;
    Entity * ent = NULL;

    raw_entity_text = "CPU;@A:in std_logic;";
    ent = getEntityFromRawEntityText(raw_entity_text);

    mu_assert(ent, "First entity wasn't parsed correctly.");
    mu_assert(strcmp(ent->name, "CPU") == 0, "First entity's name is wrong.");
    mu_assert(ent->count_in == 1, "Input signal count is wrong for first entity.");
    mu_assert(strcmp(ent->signals_in[0].length, "1") == 0, "Input signal length is wrong for first entity.");
    mu_assert(strcmp(ent->signals_in[0].name, "A") == 0, "Input signal name is wrong for first entity.");

    destroyEntity(ent);

    raw_entity_text = "MMU;@B:in std_logic_vector(5 downto 0);";
    ent = getEntityFromRawEntityText(raw_entity_text);

    mu_assert(ent, "Second entity wasn't parsed correctly.");
    mu_assert(strcmp(ent->name, "MMU") == 0, "Second entity's name is wrong.");
    mu_assert(ent->count_in == 1, "Input signal count is wrong for second entity.");
    mu_assert(strcmp(ent->signals_in[0].length, "6") == 0, "Input signal length is wrong for second entity.");
    mu_assert(strcmp(ent->signals_in[0].name, "B") == 0, "Input signal name is wrong for second entity.");

    destroyEntity(ent);

    raw_entity_text = "FlipFlop;@C:in std_logic;D:out std_logic_vector(3 downto 0);";
    ent = getEntityFromRawEntityText(raw_entity_text);

    mu_assert(ent, "Third entity wasn't parsed correctly.");
    mu_assert(strcmp(ent->name, "FlipFlop") == 0, "Third entity's name is wrong.");
    mu_assert(ent->count_in == 1, "Input signal count is wrong for third entity.");
    mu_assert(ent->count_out == 1, "Output signal count is wrong for third entity.");
    mu_assert(strcmp(ent->signals_in[0].length, "1") == 0, "Input signal length is wrong for third entity.");
    mu_assert(strcmp(ent->signals_in[0].name, "C") == 0, "Input signal name is wrong for third entity.");
    mu_assert(strcmp(ent->signals_out[0].length, "4") == 0, "Output signal length is wrong for third entity.");
    mu_assert(strcmp(ent->signals_out[0].name, "D") == 0, "Output signal name is wrong for third entity.");

    destroyEntity(ent);

    raw_entity_text = "MMU;*E: integer range 0 to 32;@F:in std_logic_vector(5 downto 0);";
    ent = getEntityFromRawEntityText(raw_entity_text);

    mu_assert(ent, "Fourth entity wasn't parsed correctly.");
    mu_assert(strcmp(ent->name, "MMU") == 0, "Fourth entity's name is wrong.");
    mu_assert(ent->count_generics == 1, "Generic input count is wrong for fourth entity.");
    mu_assert(ent->count_in == 1, "Input signal count is wrong for fourth entity.");
    mu_assert(strcmp(ent->generics[0].length, "integer range 0 to 32") == 0, "Generic input length is wrong for fourth entity.");
    mu_assert(strcmp(ent->generics[0].name, "E") == 0, "Generic input name is wrong for fourth entity.");
    mu_assert(strcmp(ent->signals_in[0].length, "6") == 0, "Input signal length is wrong for fourth entity.");
    mu_assert(strcmp(ent->signals_in[0].name, "F") == 0, "Input signal name is wrong for fourth entity.");

    destroyEntity(ent);

    return NULL;
}


char * parse_entity_from_file ()
{
    char * filename = NULL;
    Entity * ent = NULL;
    size_t i = 0;
    char * input_names[8] = { "OP", "A", "B", "C", "D", "E", "F", "G" };
    char * input_lengths[8] = { "2", "32", "32", "32", "32", "32", "32", "32" };
    char * output_names[2] = { "O", "N" };
    char * output_lengths[2] = { "32", "1" };

    filename = "tests/vhdl_sources/ALU.vhdl";
    ent = getEntityFromFile(filename);

    mu_assert(ent, "First entity wasn't parsed correctly.");
    mu_assert(strcmp(ent->name, "ALU") == 0, "First entity's name is wrong.");
    mu_assert(ent->count_in == 8, "Input signal count is wrong for the first entity.");
    mu_assert(ent->count_out == 2, "Output signal count is wrong for the first entity.");

    for (i = 0; i < ent->count_in; i++) {
        mu_assert(strcmp(ent->signals_in[i].name, input_names[i]) == 0, "First entity: an input name is wrong.");
        mu_assert(strcmp(ent->signals_in[i].length, input_lengths[i]) == 0, "First entity: an input length is wrong.");
    }

    for (i = 0; i < ent->count_out; i++) {
        mu_assert(strcmp(ent->signals_out[i].name, output_names[i]) == 0, "First entity: an output name is wrong.");
        mu_assert(strcmp(ent->signals_out[i].length, output_lengths[i]) == 0, "First entity: an output length is wrong.");
    }

    destroyEntity(ent);


    filename = "tests/vhdl_sources/ALU_oneline.vhdl";
    ent = getEntityFromFile(filename);

    mu_assert(ent, "Second entity wasn't parsed correctly.");
    mu_assert(strcmp(ent->name, "ALU") == 0, "Second entity's name is wrong.");
    mu_assert(ent->count_in == 8, "Input signal count is wrong for the second entity.");
    mu_assert(ent->count_out == 2, "Output signal count is wrong for the second entity.");

    for (i = 0; i < ent->count_in; i++) {
        mu_assert(strcmp(ent->signals_in[i].name, input_names[i]) == 0, "Second entity: an input name is wrong.");
        mu_assert(strcmp(ent->signals_in[i].length, input_lengths[i]) == 0, "Second entity: an input length is wrong.");
    }

    for (i = 0; i < ent->count_out; i++) {
        mu_assert(strcmp(ent->signals_out[i].name, output_names[i]) == 0, "Second entity: an output name is wrong.");
        mu_assert(strcmp(ent->signals_out[i].length, output_lengths[i]) == 0, "Second entity: an output length is wrong.");
    }

    destroyEntity(ent);

    return NULL;
}


char * all_tests ()
{
    mu_suite_start();

    mu_run_test(test_legal_port_name_characters);
    mu_run_test(test_alphabetic_length_strings);    
    mu_run_test(parse_simple_port_names);
    mu_run_test(parse_simple_port_directions);
    mu_run_test(parse_simple_port_lengths);
    mu_run_test(parse_raw_text_entity_from_file);
    mu_run_test(parse_entity_from_raw_entity_text);
    mu_run_test(parse_entity_from_file);

    return NULL;
}

RUN_TESTS(all_tests);
