#ifndef _CANVAS_H_
#define _CANVAS_H_
#include <goocanvas.h>


#define CANVAS_STATE(vptr) (CanvasState*) vptr
void canvas_init(GtkWidget* container, GdkRGBA* draw_color, double* line_width);

#endif
