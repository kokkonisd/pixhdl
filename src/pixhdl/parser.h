#ifndef __PARSER_H__
#define __PARSER_H__

#define ENTITY_REGEX "entity[ \\t\\r\\n\\f]*([a-zA-Z0-9_]+)[ \\t\\r\\n\\f]*is.*port[ \\t\\r\\n\\f]*\\([ \\t\\r\\n\\f]*(.+)[ \\t\\r\\n\\f]*\\);[^a-zA-Z]*end"
#define PORT_REGEX "([a-zA-Z0-9_, ]+)[ \\t\\r\\n\\f]*:[ \\t\\r\\n\\f]*(in|out|inout)[ \\t\\r\\n\\f]*(std_logic[^;]*);"
#define VECTOR_LENGTH_REGEX "([0-9]+)[^0-9]+([0-9]+)"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "entity.h"



int isLegalPortNameChar (const char portname);
char * parseSignalName (const char * raw_txt, int start, int end);
direction parseSignalDirection (const char * raw_txt, int start, int end);
int parseSignalLength (const char * raw_txt, int start, int end);
char * getRawEntityTextFromFile (const char * filename);
Entity * getEntityFromRawEntityText (const char * entity_text);
Entity * getEntityFromFile (const char * filename);

#endif
