#ifndef _CANVAS_H_
#define _CANVAS_H_
#include <goocanvas.h>

#define MAX_LINE_WIDTH 15.0
#define MIN_LINE_WIDTH 0.5

#define CANVAS_STATE(vptr) (CanvasState*) vptr
void canvas_init(GtkWidget* container, GdkRGBA* draw_color, GdkRGBA* bg_color, double* line_width);

#endif
