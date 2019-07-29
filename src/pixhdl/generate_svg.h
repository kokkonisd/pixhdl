#ifndef __GENERATE_SVG_H__
#define __GENERATE_SVG_H__

#define ENTITY_NAME_FONT_SIZE 60
#define RECTANGLE_STROKE_WIDTH 10
#define ARROW_LENGTH 200
#define PORT_NAME_FONT_SIZE (ENTITY_NAME_FONT_SIZE - 4)
#define LENGTH_FONT_SIZE (PORT_NAME_FONT_SIZE - 18)
#define ARROW_WIDTH (PORT_NAME_FONT_SIZE * 1.5)

#define SVG_HEADER "<svg version='1.1' baseProfile='full' width='%.2f' height='%.2f' xmlns='http://www.w3.org/2000/svg'>\n"

#define ARROW_DEFINITION "<defs id='arrowDefs'>\n\
<marker id='arrowEnd' fill='black' stroke-linejoin='bevel' viewBox='0 0 8000 8000' refX='250' refY='150' markerUnits='strokeWidth' markerWidth='300' markerHeight='300' orient='auto'><path id='arrowEndPath' stroke='rgb(0, 0, 0)' stroke-width='5' d='M 2 59 L 293 148 L 1 243 L 121 151 Z' /></marker>\n\
<marker id='arrowStart' fill='black' stroke-linejoin='bevel' viewBox='0 0 8000 8000' refX='50' refY='150' markerUnits='strokeWidth' markerWidth='300' markerHeight='300' orient='auto'><path stroke='rgb(0, 0, 0)' stroke-width='5' d='M 4 147 L 299 60 L 159 148 L 299 251 Z' /></marker>\n\
</defs>\n"

#define SIMPLE_ARROW_LINE "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='5' marker-end='url(#arrowEnd)' fill='transparent'/>\n"
#define DOUBLE_ARROW_LINE "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='5' marker-start='url(#arrowStart)' marker-end='url(#arrowEnd)' fill='transparent'/>\n"

#define IN_SIGNAL_NAME "<text text-anchor='end' x='%.2f' y='%.2f' font-size='%d' fill='black'>%s</text>\n"
#define OUT_SIGNAL_NAME "<text text-anchor='begin' x='%.2f' y='%.2f' font-size='%d' fill='black'>%s</text>\n"
#define INOUT_SIGNAL_NAME "<text text-anchor='end' x='%.2f' y='%.2f' font-size='%d' fill='black' transform='rotate(270 %.2f,%.2f)'>%s</text>\n"

#define HORIZONTAL_LENGTH_TEXT "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black'>%d</text>\n"
#define VERTICAL_LENGTH_TEXT "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' transform='rotate(270 %.2f,%.2f)'>%d</text>\n"
#define LENGTH_SLASH "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='3' fill='transparent' />\n"


#include "entity.h"
#include "parser.h"


float max (float a, float b);
int generateSvgFromEntity (Entity * ent, char * filename);


#endif
