#include <stdlib.h>
#include <gtk/gtk.h>

#include "canvas.h"

typedef struct CanvasState {
	GooCanvasItem* background_item;
	GooCanvasItem* root;
	GooCanvasItem* current_group;

	double* line_width;
    GdkRGBA* draw_color;

	double prev_x;
	double prev_y;
    
} CanvasState;

/*
  creates a canvas_state with default values
 */
static CanvasState* canvas_state_new(GtkWidget* goo_canvas, GdkRGBA* draw_color, double* line_width) {
	CanvasState* c = malloc(sizeof(CanvasState));

	c->root = goo_canvas_get_root_item(GOO_CANVAS(goo_canvas));
	c->background_item = goo_canvas_rect_new(c->root, 0, 0, 0, 0,
										  "line-width", 10.0,
										  "stroke-color", "white",
										  "fill-color", "white",
										  NULL);

	c->line_width  = line_width; 
	c->draw_color = draw_color;

	c->current_group = NULL;
	c->prev_x = -1;
	c->prev_y = -1;

	return c;
}

static void canvas_state_set_background_size(CanvasState* state, int width, int height) {
	GooCanvasRect* background = GOO_CANVAS_RECT(state->background_item);
	background->rect_data->width = width;
	background->rect_data->height = height;
}

static void canvas_state_prev_points_set(CanvasState* state, double x, double y) {
	state->prev_x = x;
	state->prev_y = y;
}

static void canvas_state_current_drawing_new(CanvasState* state, double x, double y) {
	state->current_group = goo_canvas_group_new(state->root,
												"line_width", *state->line_width,
												"stroke-color-gdk-rgba", state->draw_color,
												NULL);
	canvas_state_prev_points_set(state, x, y);
}

static void canvas_state_current_drawing_realize(CanvasState* state) {
	state->current_group = NULL;
	canvas_state_prev_points_set(state, -1.0, -1.0);
}

static void canvas_state_current_drawing_next_linepart(CanvasState* state, double x2, double y2) {
	if(state->current_group) {
		GooCanvasItem* _polypoint = goo_canvas_polyline_new_line(state->current_group,
																state->prev_x, state->prev_y,
																x2, y2,
																NULL);
	}
}

static void canvas_cb_unref(GtkWidget* _widget, gpointer data_canvas_state) {
	free(data_canvas_state);
}

static void canvas_cb_realize(GtkWidget* canvas, gpointer data_canvas_state) {
	/*
	  Set background to widget width/height
	  widget width/height before realize is 1
	 */
	int width = gtk_widget_get_allocated_width(canvas);
	int height = gtk_widget_get_allocated_height(canvas);
	canvas_state_set_background_size(CANVAS_STATE(data_canvas_state), width, height);
}


static gboolean canvas_cb_button_press_event(GtkWidget* widget,
										GdkEventButton* event,
										gpointer data)
{
	CanvasState* state = CANVAS_STATE(data);
	if (event->button == GDK_BUTTON_PRIMARY) {
		canvas_state_current_drawing_new(state, event->x, event->y);
	}

	// TODO Create stack based history redo/undo
	if (event->button == GDK_BUTTON_SECONDARY) {
		int len = goo_canvas_item_get_n_children(state->root);
		if (len > 1) goo_canvas_item_remove_child(state->root, len-1);
	}

	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean canvas_cb_button_release_event(GtkWidget* widget,
										GdkEventButton* event,
										gpointer data)
{
	CanvasState* state = CANVAS_STATE(data);
	if (event->button == GDK_BUTTON_PRIMARY) {
		canvas_state_current_drawing_realize(state);
	}

	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean canvas_cb_motion_notify_event(GtkWidget* widget,
									   GdkEventMotion* event,
									   gpointer data)
{
	CanvasState* state = CANVAS_STATE(data);
	if (event->state & GDK_BUTTON1_MASK) {
		canvas_state_current_drawing_next_linepart(state, event->x, event->y); 
		canvas_state_prev_points_set(state, event->x, event->y);
	}

	gtk_widget_queue_draw(widget);
	return TRUE;
}

/*
  Initlize a goo_canvas widget with callbacks to CanvasState
 */
void canvas_init(GtkWidget* container, GdkRGBA* draw_color, double* line_width) {
	GtkWidget* canvas = goo_canvas_new();
	CanvasState* canvas_state = canvas_state_new(canvas, draw_color, line_width);

	gtk_container_add(GTK_CONTAINER(container), canvas);

	gtk_widget_set_events(canvas, gtk_widget_get_events(canvas)
						  | GDK_BUTTON_PRESS_MASK
						  | GDK_POINTER_MOTION_MASK
						  | GDK_STRUCTURE_MASK);

	g_signal_connect(canvas, "realize", G_CALLBACK(canvas_cb_realize), canvas_state);
	g_signal_connect(canvas, "button-press-event", G_CALLBACK(canvas_cb_button_press_event), canvas_state);
	g_signal_connect(canvas, "button-release-event", G_CALLBACK(canvas_cb_button_release_event), canvas_state);
	g_signal_connect(canvas, "motion-notify-event", G_CALLBACK(canvas_cb_motion_notify_event), canvas_state);

	GtkWidget* window = gtk_widget_get_toplevel(canvas);
	if (!GTK_IS_WINDOW(window)) {
		g_printerr("ERROR: toplevel of canvas is not window \%s:%d\n", __FILE__, __LINE__);
		exit(2);
	}
	g_signal_connect(window, "destroy", G_CALLBACK(canvas_cb_unref), canvas_state);
}
