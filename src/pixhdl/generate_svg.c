#include "generate_svg.h"


/**
 * Returns the maximum between two floats.
 * @param  a (float): First possible maximum
 * @param  b (float): Second possible maximum
 * @return   (float): The maximum between a and b
 */
float max (float a, float b)
{
    return a > b ? a : b;
}


/**
 * Draws an arrow head into an SVG file (fo) around the point (x, y),
 * given a direction (LEFT, RIGHT, UP, DOWN).
 * @param fo  (FILE *): Output SVG file for the arrow head to be written into
 * @param x    (float): The x coordinate of the point
 * @param y    (float): The y coordinate of the point
 * @param dir (arrDir): The direction of the arrow head (LEFT, RIGHT, UP, DOWN)
 */
void drawArrowHead (FILE * fo, float x, float y, arrDir dir)
{
    // Draw arrow head based on its direction
    switch (dir) {
        case LEFT:
            fprintf(fo, ARROW_HEAD, x + ARROW_WIDTH / 8, y,
                                    x + ARROW_WIDTH / 2, y - ARROW_WIDTH / 2,
                                    x - ARROW_WIDTH,     y,
                                    x + ARROW_WIDTH / 2, y + ARROW_WIDTH / 2);
            break;

        case UP:
            fprintf(fo, ARROW_HEAD, x,                   y + ARROW_WIDTH / 8,
                                    x - ARROW_WIDTH / 2, y + ARROW_WIDTH / 2,
                                    x,                   y - ARROW_WIDTH,
                                    x + ARROW_WIDTH / 2, y + ARROW_WIDTH / 2);
            break;

        case DOWN:
            fprintf(fo, ARROW_HEAD, x,                   y - ARROW_WIDTH / 8,
                                    x - ARROW_WIDTH / 2, y - ARROW_WIDTH / 2,
                                    x,                   y + ARROW_WIDTH,
                                    x + ARROW_WIDTH / 2, y - ARROW_WIDTH / 2);
            break;

        case RIGHT:
        default:
            fprintf(fo, ARROW_HEAD, x - ARROW_WIDTH / 8, y,
                                    x - ARROW_WIDTH / 2, y - ARROW_WIDTH / 2,
                                    x + ARROW_WIDTH,     y,
                                    x - ARROW_WIDTH / 2, y + ARROW_WIDTH / 2);
    }
}


/**
 * Draws a line with a simple arrow at its end in an SVG file (fo). Only handles
 * purely horizontal/vertical lines.
 * @param fo      (FILE *): Output SVG file for the line to be written into
 * @param start_x  (float): The starting X coordinate of the line
 * @param start_y  (float): The starting Y coordinate of the line
 * @param end_x    (float): The ending X coordinate of the line
 * @param end_y    (float): The ending Y coordinate of the line
 * @param dir     (arrDir): The direction of the arrow line (LEFT, RIGHT, UP, DOWN)
 */
void drawSimpleArrowLine (FILE * fo, float start_x, float start_y, float end_x, float end_y, arrDir dir)
{
    // Draw the arrow going in the entity rect
    switch (dir) {
        case LEFT:
            fprintf(fo, SIMPLE_LINE, start_x + ARROW_WIDTH, start_y, end_x, end_y);
            drawArrowHead(fo, start_x + ARROW_WIDTH, start_y, dir);
            break;

        case UP:
            fprintf(fo, SIMPLE_LINE, start_x, start_y, end_x, end_y + ARROW_WIDTH);
            drawArrowHead(fo, end_x, end_y + ARROW_WIDTH, dir);
            break;

        case DOWN:
            fprintf(fo, SIMPLE_LINE, start_x, start_y - ARROW_WIDTH, end_x, end_y);
            drawArrowHead(fo, start_x, start_y - ARROW_WIDTH, dir);
            break;

        default:
        case RIGHT:
            fprintf(fo, SIMPLE_LINE, start_x, start_y, end_x - ARROW_WIDTH, end_y);
            drawArrowHead(fo, end_x - ARROW_WIDTH, end_y, dir);
    }

}


/**
 * Draws a line with arrows at both its ends in an SVG file (fo). Only handles
 * purely horizontal/vertical lines.
 * @param fo      (FILE *): Output SVG file for the line to be written into
 * @param start_x  (float): The starting X coordinate of the line
 * @param start_y  (float): The starting Y coordinate of the line
 * @param end_x    (float): The ending X coordinate of the line
 * @param end_y    (float): The ending Y coordinate of the line
 * @param dir     (arrDir): The direction of the arrow line (LEFT, RIGHT, UP, DOWN)
 */
void drawDoubleArrowLine (FILE * fo, float start_x, float start_y, float end_x, float end_y, arrDir dir)
{
    switch (dir) {
        case UP:
        case DOWN:
            fprintf(fo, SIMPLE_LINE, start_x, start_y - ARROW_WIDTH, end_x, end_y + ARROW_WIDTH);
            // Up-pointing arrow head
            drawArrowHead(fo, end_x, end_y + ARROW_WIDTH, UP);
            // Down-pointing arrow head
            drawArrowHead(fo, start_x, start_y - ARROW_WIDTH, DOWN);
            break;

        default:
        case RIGHT:
        case LEFT:
            fprintf(fo, SIMPLE_LINE, start_x + ARROW_WIDTH, start_y, end_x - ARROW_WIDTH, end_y);
            // Right-pointing arrow head
            drawArrowHead(fo, end_x - ARROW_WIDTH, end_y, RIGHT);
            // Left-pointing arrow head
            drawArrowHead(fo, start_x + ARROW_WIDTH, start_y, LEFT);

    }
}


/**
 * Generates an SVG image given an Entity object.
 * @param  ent      (Entity *): The entity object from which to generate an image
 * @param  filename   (char *): The name/path of the output image file
 * @return               (int): 1 if successful, 0 otherwise
 */
int generateSvgFromEntity (Entity * ent, char * filename)
{
    FILE * svg_file = NULL;
    float port_width_left = 0.0;
    float port_width_right = 0.0;
    float arrow_length_left = 0.0;
    float arrow_length_right = 0.0;
    float arrow_length_bottom = 0.0;
    float generics_height = 0.0;
    float port_width = 0.0;
    float rect_width = 0.0;
    float rect_height = 0.0;
    float svg_width = 0.0;
    float svg_height = 0.0;
    float step = 0.0;
    float cur_pos = 0.0;
    unsigned int i = 0;


    // Assert that the arguments are valid
    check(ent, "Cannot write empty entity to file.");
    check(filename, "Cannot write to empty filename.");

    // Calculate port width on both sides of the Entity rectangle
    // Each time we need to account for the maximum port name length times the approximate
    // character width (font_size * 0.55), adding 3 extra characters for spacing.
    // To that we also need to add the arrow length.
    arrow_length_left = max(ARROW_LENGTH, (ent->max_length_size_in + 2) * LENGTH_FONT_SIZE) + ARROW_WIDTH * 1.5;
    arrow_length_right = max(ARROW_LENGTH, (ent->max_length_size_out + 2) * LENGTH_FONT_SIZE) + ARROW_WIDTH * 1.5;
    port_width_left = arrow_length_left + (ent->max_name_size_in + 3) * PORT_NAME_FONT_SIZE * 0.55;
    port_width_right = arrow_length_right + (ent->max_name_size_out + 3) * PORT_NAME_FONT_SIZE * 0.55;
    // The port height is simply the width of one arrow head
    port_width = ARROW_WIDTH * 2 + LENGTH_FONT_SIZE;

    // Calculate Entity rectangle width and height
    // The width should be equal to the maximum between two floats:
    //     1. The entity's name width (in pixels, so as before that is font_size * 0.55) plus 2 characters for spacing
    //     2. The width produced by the INOUT channels, if they exist
    rect_width = max(ENTITY_NAME_FONT_SIZE * (strlen(ent->name) + 2), port_width * ent->count_inout);
    if (ent->count_generics > 0)
        rect_width += (ent->max_name_size_generics + ent->max_length_size_generics + 2) * PORT_NAME_FONT_SIZE * 0.55;
    // The height of the rectangle is also the maximum between two floats:
    //     1. The maximum between INs and OUTs times the width of one arrow head
    //     2. The entity's name font size (which basically means "enough place for the entity's name")
    rect_height = max(max(ent->count_in, ent->count_out) * port_width, ENTITY_NAME_FONT_SIZE);
    generics_height = (ent->count_generics + 1) * PORT_NAME_FONT_SIZE;
    if (ent->count_generics > 0)
        rect_height += generics_height;

    // The total SVG width is then simply the sum of the two port widths plus the entity rect width
    svg_width = rect_width + RECTANGLE_STROKE_WIDTH * 2 + port_width_left + port_width_right;

    // The total SVG height is at least equal to the entity rect height plus its stroke width
    svg_height = rect_height + RECTANGLE_STROKE_WIDTH;

    // If there are INOUT signals, we need to add their height too
    arrow_length_bottom = max(ARROW_LENGTH, (ent->max_length_size_inout + 2) * LENGTH_FONT_SIZE) + ARROW_WIDTH;
    if (ent->count_inout > 0)
        svg_height += arrow_length_bottom + (PORT_NAME_FONT_SIZE * 0.55 * (ent->max_name_size_inout + 2));

    // Open the output file, we're ready to produce the image
    svg_file = fopen(filename, "w");

    // First off, print the SVG header along with the width and height of the image
    fprintf(svg_file, SVG_HEADER, svg_width, svg_height);

    // Draw the entity rectangle
    fprintf(svg_file, ENTITY_RECTANGLE, rect_width,
                                        rect_height,
                                        port_width_left + RECTANGLE_STROKE_WIDTH / 2,
                                        (float)(RECTANGLE_STROKE_WIDTH / 2),
                                        RECTANGLE_STROKE_WIDTH);

    // Draw the entity's name inside the rectangle
    fprintf(svg_file, ENTITY_NAME, port_width_left + rect_width / 2,
                                   rect_height - RECTANGLE_STROKE_WIDTH / 2 - ENTITY_NAME_FONT_SIZE * 0.25,
                                   ENTITY_NAME_FONT_SIZE,
                                   ent->name);


    // Draw the GENERIC inputs
    for (i = 0; i < ent->count_generics; i++) {
        fprintf(svg_file, GENERIC_SIGNAL_NAME, port_width_left + RECTANGLE_STROKE_WIDTH + PORT_NAME_FONT_SIZE,
                                               (float)(RECTANGLE_STROKE_WIDTH + PORT_NAME_FONT_SIZE) * (i + 1),
                                               PORT_NAME_FONT_SIZE,
                                               ent->generics[i].name,
                                               ent->generics[i].length);

        fprintf(svg_file, LENGTH_SLASH, port_width_left,
                                        (float)(RECTANGLE_STROKE_WIDTH + PORT_NAME_FONT_SIZE) * (i + 1.25),
                                        port_width_left + rect_width,
                                        (float)(RECTANGLE_STROKE_WIDTH + PORT_NAME_FONT_SIZE) * (i + 1.25));
    }


    // Draw the IN signals
    //
    // We need to establish a step (aka how much space is vertically allocated to each signal)
    // as well as a cur_pos, which will tell us where to draw the next signal vertically.
    // The step is simply the rectangle's height divided by the number of IN signals,
    // while the initial Y position is equal to half a step plus an offset due to the stroke width
    // of the entity rect.
    step = rect_height / ent->count_in;
    cur_pos = RECTANGLE_STROKE_WIDTH / 2 + step / 2;

    // Loop over the IN signals and draw them
    for (i = 0; i < ent->count_in; i++) {
        // Draw the arrow going in the entity rect
        drawSimpleArrowLine(svg_file, port_width_left - arrow_length_left,
                                      cur_pos,
                                      port_width_left,
                                      cur_pos,
                                      RIGHT);

        // Draw a slash to indicate signal length in bits
        fprintf(svg_file, LENGTH_SLASH, port_width_left - arrow_length_left / 2 - 10,
                                        cur_pos + 10,
                                        port_width_left - arrow_length_left / 2 + 10,
                                        cur_pos - 10);

        // Draw the signal's name
        fprintf(svg_file, IN_SIGNAL_NAME, port_width_left - arrow_length_left - PORT_NAME_FONT_SIZE * 0.55,
                                          cur_pos + PORT_NAME_FONT_SIZE * 0.25,
                                          PORT_NAME_FONT_SIZE,
                                          ent->signals_in[i].name);


        // Draw the signal's length above the slash we drew earlier
        fprintf(svg_file, HORIZONTAL_LENGTH_TEXT, port_width_left - arrow_length_left / 2,
                                       cur_pos - 15,
                                       LENGTH_FONT_SIZE,
                                       ent->signals_in[i].length);

        // Move the current Y position forward by one step
        cur_pos += step;
    }


    // Draw the OUT signals
    //
    // We need to establish a step (aka how much space is vertically allocated to each signal)
    // as well as a cur_pos, which will tell us where to draw the next signal vertically.
    // The step is simply the rectangle's height divided by the number of OUT signals,
    // while the initial Y position is equal to half a step plus an offset due to the stroke width
    // of the entity rect.
    step = rect_height / ent->count_out;
    cur_pos = RECTANGLE_STROKE_WIDTH / 2 + step / 2;

    // Loop over the OUT signals and draw them
    for (i = 0; i < ent->count_out; i++) {
        // Draw the arrow coming out of the entity rect
        drawSimpleArrowLine(svg_file, port_width_left + rect_width,
                                      cur_pos,
                                      port_width_left + rect_width + arrow_length_right,
                                      cur_pos,
                                      RIGHT);

        // Draw a slash to indicate signal length in bits
        fprintf(svg_file, LENGTH_SLASH, port_width_left + rect_width + arrow_length_right / 2 - 10,
                                        cur_pos + 10,
                                        port_width_left + rect_width + arrow_length_right / 2 + 10,
                                        cur_pos - 10);

        // Draw the signal's name
        fprintf(svg_file, OUT_SIGNAL_NAME, port_width_left + rect_width + arrow_length_right + PORT_NAME_FONT_SIZE * 0.55,
                                           cur_pos + PORT_NAME_FONT_SIZE * 0.25,
                                           PORT_NAME_FONT_SIZE,
                                           ent->signals_out[i].name);

        // Draw the signal's length above the slash we drew earlier
        fprintf(svg_file, HORIZONTAL_LENGTH_TEXT, port_width_left + rect_width + arrow_length_right / 2,
                                                  cur_pos - 15,
                                                  LENGTH_FONT_SIZE,
                                                  ent->signals_out[i].length);

        // Move the current Y position forward by one step
        cur_pos += step;
    }


    // Draw the INOUT signals
    //
    // We need to establish a step (aka how much space is horizontally allocated to each signal)
    // as well as a cur_pos, which will tell us where to draw the next signal horizontally.
    // The step is simply the rectangle's width divided by the number of INOUT signals,
    // while the initial X position is equal to half a step plus an offset due to the stroke width
    // of the entity rect.
    step = rect_width / ent->count_inout;
    cur_pos = port_width_left + step / 2;

    // Loop over the INOUT signals and draw them
    for (i = 0; i < ent->count_inout; i++) {
        // Draw the arrow going in and coming out of the entity rect
        drawDoubleArrowLine(svg_file, cur_pos,
                                      svg_height - (ent->max_name_size_inout + 2) * PORT_NAME_FONT_SIZE * 0.55,
                                      cur_pos,
                                      rect_height + RECTANGLE_STROKE_WIDTH,
                                      DOWN);

        // Draw a slash to indicate signal length in bits
        fprintf(svg_file, LENGTH_SLASH, cur_pos - 10,
                                        rect_height + RECTANGLE_STROKE_WIDTH + arrow_length_bottom / 2 - 10,
                                        cur_pos + 10,
                                        rect_height + RECTANGLE_STROKE_WIDTH + arrow_length_bottom / 2 + 10);

        // Draw the signal's name
        fprintf(svg_file, INOUT_SIGNAL_NAME, cur_pos + PORT_NAME_FONT_SIZE * 0.25,
                                             svg_height - (ent->max_name_size_inout + 1) * PORT_NAME_FONT_SIZE * 0.55,
                                             PORT_NAME_FONT_SIZE,
                                             cur_pos + PORT_NAME_FONT_SIZE * 0.25,
                                             svg_height - (ent->max_name_size_inout + 1) * PORT_NAME_FONT_SIZE * 0.55,
                                             ent->signals_inout[i].name);

        // Draw the signal's length above the slash we drew earlier
        fprintf(svg_file, VERTICAL_LENGTH_TEXT, cur_pos - 15,
                                                rect_height + RECTANGLE_STROKE_WIDTH + arrow_length_bottom / 2,
                                                LENGTH_FONT_SIZE,
                                                cur_pos - 15,
                                                rect_height + RECTANGLE_STROKE_WIDTH + arrow_length_bottom / 2,
                                                ent->signals_inout[i].length);

        // Move the current X position forward by one step
        cur_pos += step;
    }


    // Add the SVG closing tag
    fprintf(svg_file, "</svg>\n");

    // Close the file
    fclose(svg_file);

    return 0;

error:
    return 1;
}
