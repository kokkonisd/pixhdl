#ifndef __PARSER_H__
#define __PARSER_H__

#define ENTITY_REGEX "entity.*is.*port[ \t]*\\([\n]*(.+)[\n]*\\);[^a-zA-Z]*end"
#define PORT_REGEX "([a-zA-Z0-9_, ]+)[ \\t\\r\\n\\f]*:[ \\t\\r\\n\\f]*(in|out|inout)[ \\t\\r\\n\\f]*(std_logic[^;]*);"

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
