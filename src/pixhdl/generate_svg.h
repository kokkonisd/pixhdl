#ifndef __GENERATE_SVG_H__
#define __GENERATE_SVG_H__

#define ENTITY_NAME_FONT_SIZE 60
#define RECTANGLE_STROKE_WIDTH 10
#define ARROW_LENGTH 200
#define PORT_NAME_FONT_SIZE (ENTITY_NAME_FONT_SIZE - 4)
#define LENGTH_FONT_SIZE (PORT_NAME_FONT_SIZE - 18)
#define ARROW_WIDTH 30

#define SVG_HEADER "<svg version='1.1' baseProfile='full' width='%.2f' height='%.2f' xmlns='http://www.w3.org/2000/svg'>\n"

#define ENTITY_RECTANGLE "<rect width='%.2f' height='%.2f' x='%.2f' y='%.2f' fill='none' stroke='black' stroke-width='%d' />\n"
#define ENTITY_NAME "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"

#define ARROW_HEAD "<path d='M%.2f %.2f L%.2f %.2f L%.2f %.2f L%.2f %.2f Z'/>\n"

#define SIMPLE_LINE "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='5' fill='transparent'/>\n"

#define GENERIC_SIGNAL_NAME "<text text-anchor='begin' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s : %s</text>\n"
#define IN_SIGNAL_NAME "<text text-anchor='end' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"
#define OUT_SIGNAL_NAME "<text text-anchor='begin' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"
#define INOUT_SIGNAL_NAME "<text text-anchor='end' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier' transform='rotate(270 %.2f,%.2f)'>%s</text>\n"

#define HORIZONTAL_LENGTH_TEXT "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"
#define VERTICAL_LENGTH_TEXT "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier' transform='rotate(270 %.2f,%.2f)'>%s</text>\n"
#define LENGTH_SLASH "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='3' fill='transparent' />\n"


#include "entity.h"
#include "parser.h"


typedef enum {
    RIGHT,
    LEFT,
    UP,
    DOWN
} arrDir;

float max (float a, float b);
int generateSvgFromEntity (Entity * ent, char * filename);


#endif
