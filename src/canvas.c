#include <stdlib.h>
#include <gtk/gtk.h>
#include <goocanvas.h>

#include "canvas.h"

typedef struct CanvasState {
	GooCanvasItem* background_item;
	GooCanvasItem* root;
} CanvasState;

static CanvasState* canvas_state_new(GtkWidget* goo_canvas) {
	CanvasState* c = malloc(sizeof(CanvasState));


	c->root = goo_canvas_get_root_item(GOO_CANVAS(goo_canvas));
	c->background_item = goo_canvas_rect_new(c->root, 0, 0, 0, 0,
										  "line-width", 10.0,
										  "stroke-color", "white",
										  "fill-color", "white",
										  NULL);
	return c;
}

void canvas_cb_unref(GtkWidget* _widget, gpointer canvas) {
	free(canvas);
}

void canvas_cb_realize(GtkWidget* canvas, gpointer data_canvas_state) {

	/*
	  Set background to widget width/height
	  widget width/height before realize is 1
	 */
	int width = gtk_widget_get_allocated_width(canvas);
	int height = gtk_widget_get_allocated_height(canvas);
	CanvasState* state = (CanvasState*) data_canvas_state;
	GooCanvasRect* background = GOO_CANVAS_RECT(state->background_item);
	background->rect_data->width = width;
	background->rect_data->height = height;

}

/*
  Initlize a goo_canvas widget with callbacks to CanvasState
 */
void canvas_init(GtkWidget* container) {
	GtkWidget* canvas = goo_canvas_new();
	CanvasState* canvas_state = canvas_state_new(canvas);

	gtk_container_add(GTK_CONTAINER(container), canvas);

	gtk_widget_set_events(canvas, gtk_widget_get_events(canvas)
						  | GDK_BUTTON_PRESS_MASK
						  | GDK_POINTER_MOTION_MASK
						  | GDK_STRUCTURE_MASK);

	g_signal_connect(canvas, "realize", G_CALLBACK(canvas_cb_realize), canvas_state);

	GtkWidget* window = gtk_widget_get_toplevel(canvas);
	if (!GTK_IS_WINDOW(window)) {
		g_printerr("ERROR: toplevel of canvas is not window \%s:%d\n", __FILE__, __LINE__);
		exit(2);
	}
	g_signal_connect(window, "destroy", G_CALLBACK(canvas_cb_unref), canvas_state);

}
