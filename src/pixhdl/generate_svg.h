/**
 * @file generate_svg.h
 * @author Dimitri Kokkonis ([\@kokkonisd](https://github.com/kokkonisd))
 * @date 4 Aug 2019
 * @brief SVG image file generation functionality for the _pixhdl_ project.
 */


#ifndef __GENERATE_SVG_H__
#define __GENERATE_SVG_H__

/** The font size for the Entity name. */
#define ENTITY_NAME_FONT_SIZE 60
/** The font size for the Signal names. */
#define PORT_NAME_FONT_SIZE (ENTITY_NAME_FONT_SIZE - 4)
/** The font size for the lengths/values of the signals. */
#define LENGTH_FONT_SIZE (PORT_NAME_FONT_SIZE - 18)
/** The stroke width of the Entity rectangle. */
#define RECTANGLE_STROKE_WIDTH 10
/** The length of the arrow lines of the ports. */
#define ARROW_LENGTH 200
/** The width of the arrow heads. */
#define ARROW_WIDTH 30

/** The header of the SVG output file. */
#define SVG_HEADER "<svg version='1.1' baseProfile='full' width='%.2f' height='%.2f' xmlns='http://www.w3.org/2000/svg'>\n"

/** The Entity rectangle. */
#define ENTITY_RECTANGLE "<rect width='%.2f' height='%.2f' x='%.2f' y='%.2f' fill='none' stroke='black' stroke-width='%d' />\n"
/** The Entity name text. */
#define ENTITY_NAME "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"

/** The arrow head path. */
#define ARROW_HEAD "<path d='M%.2f %.2f L%.2f %.2f L%.2f %.2f L%.2f %.2f Z'/>\n"
/** A simple line connecting two points. */
#define SIMPLE_LINE "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='5' fill='transparent'/>\n"

#define CLOCK_TRIANGLE "<path d='M%.2f %.2f L%.2f %.2f L%.2f %.2f Z' stroke='black' stroke-width='3' fill='transparent'/>\n"

/** The text for the generic input names. */
#define GENERIC_SIGNAL_NAME "<text text-anchor='begin' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s : %s</text>\n"
/** The text for the IN signal names. */
#define IN_SIGNAL_NAME "<text text-anchor='end' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"
/** The text for the OUT signal names. */
#define OUT_SIGNAL_NAME "<text text-anchor='begin' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"
/** The text for the INOUT signal names. */
#define INOUT_SIGNAL_NAME "<text text-anchor='end' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier' transform='rotate(270 %.2f,%.2f)'>%s</text>\n"

/** The text for the horizontal input/output lengths. */
#define HORIZONTAL_LENGTH_TEXT "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier'>%s</text>\n"
/** The text for the vertical throughput lengths. */
#define VERTICAL_LENGTH_TEXT "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black' font-family='Courier' transform='rotate(270 %.2f,%.2f)'>%s</text>\n"
/** The small line (slash) marking the length of a signal. */
#define LENGTH_SLASH "<line x1='%.2f' y1='%.2f' x2='%.2f' y2='%.2f' stroke='black' stroke-width='3' fill='transparent' />\n"


#include "entity.h"
#include "parser.h"


/** Enumeration of the possible directions of an arrow line/arrow head (`LEFT`, `RIGHT`, `UP`, `DOWN`). */
typedef enum {
    RIGHT,
    LEFT,
    UP,
    DOWN
} arrDir;

/**
 * Returns the maximum between two floats.
 *
 * @param  a (`float`): First possible maximum.
 * @param  b (`float`): Second possible maximum.
 * @return   (`float`): The maximum between `a` and `b`.
 */
float max (float a, float b);

/**
 * Draws an arrow head into an SVG file (fo) around the point (x, y),
 * given a direction (`LEFT`, `RIGHT`, `UP`, `DOWN`).
 *
 * @param fo  (`FILE *`): Output SVG file for the arrow head to be written into.
 * @param x    (`float`): The `x` coordinate of the point.
 * @param y    (`float`): The `y` coordinate of the point
 * @param dir (`arrDir`): The direction of the arrow head (`LEFT`, `RIGHT`, `UP`, `DOWN`).
 */
void drawArrowHead (FILE * fo, float x, float y, arrDir dir);

/**
 * Draws a line with a simple arrow at its end in an SVG file (fo). Only handles
 * purely horizontal/vertical lines.
 *
 * @param fo      (`FILE *`): Output SVG file for the line to be written into.
 * @param start_x  (`float`): The starting X coordinate of the line.
 * @param start_y  (`float`): The starting Y coordinate of the line.
 * @param end_x    (`float`): The ending X coordinate of the line.
 * @param end_y    (`float`): The ending Y coordinate of the line.
 * @param dir     (`arrDir`): The direction of the arrow line (`LEFT`, `RIGHT`, `UP`, `DOWN`).
 */
void drawSimpleArrowLine (FILE * fo, float start_x, float start_y, float end_x, float end_y, arrDir dir);

/**
 * Draws a line with arrows at both its ends in an SVG file (fo). Only handles
 * purely horizontal/vertical lines.
 *
 * @param fo      (`FILE *`): Output SVG file for the line to be written into.
 * @param start_x  (`float`): The starting X coordinate of the line.
 * @param start_y  (`float`): The starting Y coordinate of the line.
 * @param end_x    (`float`): The ending X coordinate of the line.
 * @param end_y    (`float`): The ending Y coordinate of the line.
 * @param dir     (`arrDir`): The direction of the arrow line (`LEFT`, `RIGHT`, `UP`, `DOWN`).
 */
void drawDoubleArrowLine (FILE * fo, float start_x, float start_y, float end_x, float end_y, arrDir dir);

/**
 * Generates an SVG image given an Entity object.
 *
 * @param  ent      (`Entity *`): The entity object from which to generate an image.
 * @param  filename   (`char *`): The name/path of the output image file.
 * @return               (`int`): 1 if successful, 0 otherwise.
 */
int generateSvgFromEntity (Entity * ent, char * filename);


#endif
