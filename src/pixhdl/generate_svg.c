#include "generate_svg.h"


#define SVG_STRING "<svg version=\"1.1\"\
     baseProfile=\"full\"\
     width=\"300\" height=\"200\"\
     xmlns=\"http://www.w3.org/2000/svg\">\
  <rect width=\"100%\" height=\"100%\" fill=\"red\" />\
  <circle cx=\"150\" cy=\"100\" r=\"80\" fill=\"green\" />\
  <text x=\"150\" y=\"125\" font-size=\"60\" text-anchor=\"middle\" fill=\"white\">SVG</text>\
</svg>"

int generateSvgFromEntity (Entity * ent, char * filename)
{
    FILE * svg_file = NULL;

    svg_file = fopen(filename, "w");

    fprintf(svg_file, "%s\n", SVG_STRING);

    fclose(svg_file);

    return 0;
}
