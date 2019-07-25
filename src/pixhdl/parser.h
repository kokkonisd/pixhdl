#ifndef __PARSER_H__
#define __PARSER_H__

#define ENTITY_REGEX "entity[ \t\r\n\f]+([a-zA-Z0-9_]+)[ \t\r\n\f]+is[ \t\r\n\f]+port[ \t\r\n\f]*\\([ \t\r\n\f]*([a-zA-Z0-9:;,_() \t\r\n\f]+\\)?)[ \t\r\n\f]*\\)[ \t\r\n\f]*;[ \t\r\n\f]*end"
#define PORT_REGEX "([a-zA-Z0-9_, ]+)[ \t\r\n\f]*:[ \t\r\n\f]*(in|out|inout)[ \t\r\n\f]*(std_logic[^;]*);"
#define VECTOR_LENGTH_REGEX "([0-9]+)[^0-9]+([0-9]+)"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "entity.h"


/**
 * Check if a given character is a legal port name in VHDL.
 * @param  portname_char (const char): A portname character
 * @return                      (int): 1 if the char is valid, 0 otherwise
 */
int isLegalPortNameChar (const char portname);

/**
 * Parses the name of a signal given some raw entity text, a start and an end index.
 * @param  raw_txt (const char *): The raw entity text
 * @param  start            (int): The beginning index for the raw entity text
 * @param  end              (int): The ending index for the raw entity text
 * @return               (char *): The resulting signal name
 */
char * parseSignalName (const char * raw_txt, int start, int end);

/**
 * Parses the direction of a signal given some raw entity text, a start and an end index.
 * @param  raw_txt (const char *): The raw entity text
 * @param  start            (int): The beginning index for the raw entity text
 * @param  end              (int): The ending index for the raw entity text
 * @return       (direction enum): The resulting direction
 */
direction parseSignalDirection (const char * raw_txt, int start, int end);

/**
 * Parses the length of a signal (in bits) given some raw entity text, a start and an end index.
 * @param  raw_txt (const char *): The raw entity text
 * @param  start            (int): The beginning index for the raw entity text
 * @param  end              (int): The ending index for the raw entity text
 * @return                  (int): The resulting length (in bits)
 */
int parseSignalLength (const char * raw_txt, int start, int end);

/**
 * Gets the raw entry definition from a given file. Output also contains the entity's name.
 * @param  filename (const char *): The path/name of the source file
 * @return                (char *): The raw name and body of the entity definition ("name;body")
 */
char * getRawEntityTextFromFile (const char * filename);

/**
 * Generate an Entity object given an entity definition in raw text.
 * @param  entity_text (const char *): The raw entity definition (formatted as "name;body")
 * @return                 (Entity *): The Entity object
 */
Entity * getEntityFromRawEntityText (const char * entity_text);

/**
 * Generates an Entity object given a filename.
 * @param  filename (const char *): The path/name of the source file
 * @return              (Entity *): The Entity object
 */
Entity * getEntityFromFile (const char * filename);

#endif
