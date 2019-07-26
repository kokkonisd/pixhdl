#include "generate_svg.h"


#define TST_MSG "I say: %s\n"


int max (int a, int b)
{
    return a > b ? a : b;
}


int generateSvgFromEntity (Entity * ent, char * filename)
{
    FILE * svg_file = NULL;
    int svg_width = 0;
    int svg_height = 0;
    int rect_width = 0;
    int rect_height = 0;
    int port_width = 0;
    int port_height = 0;

    port_width = ARROW_LENGTH + (ent->max_name_size + 1) * PORT_NAME_FONT_SIZE;
    port_height = ARROW_WIDTH;

    rect_width = max(ENTITY_NAME_FONT_SIZE * (strlen(ent->name) + 2), ARROW_WIDTH * ent->count_inout);
    rect_height = max(max(ent->count_in, ent->count_out) * ARROW_WIDTH, ENTITY_NAME_FONT_SIZE * (strlen(ent->name) + 2));

    svg_width = rect_width + port_width * 2;

    svg_height = rect_height + RECTANGLE_STROKE_WIDTH;
    if (ent->count_inout)
        svg_height += ARROW_WIDTH + (PORT_NAME_FONT_SIZE * (ent->max_name_size + 2));

    printf("%d %d\n", svg_width, svg_height);

    svg_file = fopen(filename, "w");

    fprintf(svg_file, SVG_HEADER, svg_width, svg_height);
    fprintf(svg_file, "<rect width='100%%' height='100%%' fill='red' />\n");
    fprintf(svg_file, "<rect width='%d' height='%d' x='%d' y='%d' fill='none' stroke='black' stroke-width='%d' />\n",
                      rect_width, rect_height, port_width, RECTANGLE_STROKE_WIDTH / 2, RECTANGLE_STROKE_WIDTH);
    fprintf(svg_file, "<text text-anchor='middle' x='%d' y='%d' font-size='%d' fill='white'>%s</text>\n",
                      port_width + rect_width / 2, rect_height / 2 + RECTANGLE_STROKE_WIDTH / 2,
                      ENTITY_NAME_FONT_SIZE, ent->name);
    fprintf(svg_file, "</svg>\n");

    fclose(svg_file);

    return 0;
}
