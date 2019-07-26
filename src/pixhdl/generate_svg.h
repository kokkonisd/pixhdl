#ifndef __GENERATE_SVG_H__
#define __GENERATE_SVG_H__

#define ENTITY_NAME_FONT_SIZE 60
#define RECTANGLE_STROKE_WIDTH 10
#define ARROW_WIDTH 50
#define ARROW_LENGTH 200
#define PORT_NAME_FONT_SIZE (ENTITY_NAME_FONT_SIZE - 4)

#define SVG_HEADER "<svg version='1.1' baseProfile='full' width='%d' height='%d' xmlns='http://www.w3.org/2000/svg'>\n"

#include "entity.h"
#include "parser.h"


int generateSvgFromEntity (Entity * ent, char * filename);


#endif
