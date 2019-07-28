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
    float port_height = 0.0;
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
    // character width (font_size * 0.55), adding 1 extra character on either side for spacing.
    // To that we also need to add the arrow length.
    port_width_left = ARROW_LENGTH + (ent->max_name_size_in + 2) * PORT_NAME_FONT_SIZE * 0.55;
    port_width_right = ARROW_LENGTH + (ent->max_name_size_out + 2) * PORT_NAME_FONT_SIZE * 0.55;
    // The port height is simply the width of one arrow head
    port_height = ARROW_WIDTH;

    // Calculate Entity rectangle width and height
    // The width should be equal to the maximum between two floats:
    //     1. The entity's name width (in pixels, so as before that is font_size * 0.55) plus 2 characters for spacing
    //     2. The width produced by the INOUT channels, if they exist
    rect_width = max(ENTITY_NAME_FONT_SIZE * (strlen(ent->name) + 2), ARROW_WIDTH * ent->count_inout);
    // The height of the rectangle is also the maximum between two floats:
    //     1. The maximum between INs and OUTs times the width of one arrow head
    //     2. The entity's name font size (which basically means "enough place for the entity's name")
    rect_height = max(max(ent->count_in, ent->count_out) * ARROW_WIDTH, ENTITY_NAME_FONT_SIZE);

    // The total SVG width is then simply the sum of the two port widths plus the entity rect width
    svg_width = rect_width + port_width_left + port_width_right;

    // The total SVG height is at least equal to the entity rect height plus its stroke width
    svg_height = rect_height + RECTANGLE_STROKE_WIDTH;
    // If there are INOUT signals, we need to add their height too
    if (ent->count_inout)
        svg_height += ARROW_WIDTH + (PORT_NAME_FONT_SIZE * 0.55 * (ent->max_name_size_inout + 2));

    // Open the output file, we're ready to produce the image
    svg_file = fopen(filename, "w");

    // First off, print the SVG header along with the width and height of the image
    fprintf(svg_file, SVG_HEADER, svg_width, svg_height);
    // Add the <defs> that help us with the arrows
    fprintf(svg_file, "%s\n", ARROW_DEFINITION);

    // Draw the entity rectangle
    fprintf(svg_file, "<rect width='%.2f' height='%.2f' x='%.2f' y='%.2f' fill='none' stroke='black' stroke-width='%d' />\n",
                      rect_width,
                      rect_height,
                      port_width_left,
                      (float)(RECTANGLE_STROKE_WIDTH / 2),
                      RECTANGLE_STROKE_WIDTH);

    // Draw the entity's name inside the rectangle
    fprintf(svg_file, "<text text-anchor='middle' x='%.2f' y='%.2f' font-size='%d' fill='black'>%s</text>\n",
                      port_width_left + rect_width / 2,
                      rect_height / 2 + RECTANGLE_STROKE_WIDTH / 2 + ENTITY_NAME_FONT_SIZE * 0.25,
                      ENTITY_NAME_FONT_SIZE,
                      ent->name);


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
        fprintf(svg_file, SIMPLE_ARROW_LINE, (ent->max_name_size_in + 2) * PORT_NAME_FONT_SIZE * 0.55,
                                         cur_pos,
                                         port_width_left - RECTANGLE_STROKE_WIDTH,
                                         cur_pos);

        // Draw a slash to indicate signal length in bits
        fprintf(svg_file, LENGTH_SLASH, port_width_left - ARROW_LENGTH / 2 - 10,
                                        cur_pos + 10,
                                        port_width_left - ARROW_LENGTH / 2 + 10,
                                        cur_pos - 10);

        // Draw the signal's name
        fprintf(svg_file, IN_SIGNAL_NAME, (ent->max_name_size_in + 1) * PORT_NAME_FONT_SIZE * 0.55,
                                          cur_pos + PORT_NAME_FONT_SIZE * 0.25,
                                          PORT_NAME_FONT_SIZE,
                                          ent->signals_in[i].name);


        // Draw the signal's length above the slash we drew earlier
        fprintf(svg_file, LENGTH_TEXT, port_width_left - ARROW_LENGTH / 2,
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
        fprintf(svg_file, SIMPLE_ARROW_LINE, svg_width - port_width_right,
                                         cur_pos,
                                         svg_width - (ent->max_name_size_out + 2) * PORT_NAME_FONT_SIZE * 0.55,
                                         cur_pos);

        // Draw a slash to indicate signal length in bits
        fprintf(svg_file, LENGTH_SLASH, svg_width - port_width_right + ARROW_LENGTH / 2 - 10,
                                        cur_pos + 10,
                                        svg_width - port_width_right + ARROW_LENGTH / 2 + 10,
                                        cur_pos - 10);

        // Draw the signal's name
        fprintf(svg_file, OUT_SIGNAL_NAME, svg_width - (ent->max_name_size_out + 1) * PORT_NAME_FONT_SIZE * 0.55,
                                           cur_pos + PORT_NAME_FONT_SIZE * 0.25,
                                           PORT_NAME_FONT_SIZE,
                                           ent->signals_out[i].name);

        // Draw the signal's length above the slash we drew earlier
        fprintf(svg_file, LENGTH_TEXT, svg_width - port_width_right + ARROW_LENGTH / 2,
                                       cur_pos - 15,
                                       LENGTH_FONT_SIZE,
                                       ent->signals_out[i].length);

        // Move the current Y position forward by one step
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