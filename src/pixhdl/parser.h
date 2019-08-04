/**
 * @file parser.h
 * @author Dimitri Kokkonis ([\@kokkonisd](https://github.com/kokkonisd))
 * @date 4 Aug 2019
 * @brief HDL file parser for the _pixhdl_ project.
 */


#ifndef __PARSER_H__
#define __PARSER_H__

/** Regex to match the entity definition of a VHDL source file. */
#define ENTITY_REGEX "entity[ \t\r\n\f]+([a-zA-Z0-9_]+)[ \t\r\n\f]+is[ \t\r\n\f]+(generic[ \t\r\n\f]*\\([ \t\r\n\f]*(.+)[ \t\r\n\f]*\\)[ \t\r\n\f]*;)?[ \t\r\n\f]*port[ \t\r\n\f]*\\([ \t\r\n\f]*(.+\\)?)[ \t\r\n\f]*\\)[ \t\r\n\f]*;[ \t\r\n\f]*end[ \t]*(entity|[a-zA-Z0-9_]+)?[ \t]*;[ \t\r\n\f]*architecture"

/** Regex to match the port definition of a VHDL source file. */
#define PORT_REGEX "([a-zA-Z0-9_, ]+)[ \t\r\n\f]*:[ \t\r\n\f]*((in|out|inout)[ \t\r\n\f]*(std_logic[^;]*)|([a-zA-Z0-9_,. \t-]*));"

/** Regex to match the vector length definition of a VHDL source file. */
#define VECTOR_LENGTH_REGEX "(\\(([a-zA-Z0-9_ -]+)[ \t\r\n\f]+(down)?to[ \t\r\n\f]+([a-zA-Z0-9_ -]+)\\))"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "entity.h"


/**
 * Check if a given character is a legal port name in VHDL.
 *
 * @param  portname_char (`const char`): A portname character.
 * @return                      (`int`): 1 if the char is valid, 0 otherwise.
 */
int isLegalPortNameChar (const char portname_char);

/**
 * Check if a given string contains any alphabetic characters.
 *
 * @param  str (`char *`): The input string to be checked.
 * @return        (`int`): 1 if the string contains any alphabetic characters, 0 otherwise.
 */
int containsAlpha (const char * str);

/**
 * Parses the name of a signal given some raw entity text, a start and an end index.
 *
 * @param  raw_txt (`const char *`): The raw entity text.
 * @param  start            (`int`): The beginning index for the raw entity text.
 * @param  end              (`int`): The ending index for the raw entity text.
 * @return               (`char *`): The resulting signal name.
 */
char * parseSignalName (const char * raw_txt, int start, int end);

/**
 * Parses the direction of a signal given some raw entity text, a start and an end index.
 *
 * @param  raw_txt (`const char *`): The raw entity text.
 * @param  start            (`int`): The beginning index for the raw entity text.
 * @param  end              (`int`): The ending index for the raw entity text.
 * @return            (`direction`): The resulting direction.
 */
direction parseSignalDirection (const char * raw_txt, int start, int end);

/**
 * Parses the length of a signal (in bits) given some raw entity text, a start and an end index.
 *
 * @param  raw_txt (`const char *`): The raw entity text.
 * @param  start            (`int`): The beginning index for the raw entity text.
 * @param  end              (`int`): The ending index for the raw entity text.
 * @return                  (`int`): The resulting length (in bits).
 */
char * parseSignalLength (const char * raw_txt, int start, int end);

/**
 * @brief Gets the raw entry definition from a given file. Output also contains the entity's name.
 *
 * Given an HDL source file, it extracts & returns the name of the entity, its generic inputs (if any)
 * and its port inputs/outputs/throughputs. The entity's name is immediately followed by a `;` separator.
 * In the case that the entity contains any generic inputs, the first of them is preceded by a `*` character.
 * The first port of the entity is preceded by a `@` character. The final string returned by the function
 * is formatted as follows:
 *
 * `EntityName;*Generic1:value;Generic2:value;...;@Port1:value;Port2:value;...;`
 *
 * @param  filename (`const char *`): The path/name of the source file.
 * @return                (`char *`): The raw name and body of the entity definition.
 */
char * getRawEntityTextFromFile (const char * filename);

/**
 * Generate an Entity object given an entity definition in raw text.
 * @see getRawEntityTextFromFile
 *
 * @param  entity_text (`const char *`): The raw entity definition.
 * @return                 (`Entity *`): The Entity object.
 */
Entity * getEntityFromRawEntityText (const char * entity_text);

/**
 * Generates an Entity object given a filename.
 *
 * @param  filename (`const char *`): The path/name of the source file.
 * @return              (`Entity *`): The Entity object.
 */
Entity * getEntityFromFile (const char * filename);

#endif
