#include <stdlib.h>
#include <gtk/gtk.h>

#include "canvas.h"
#include "config.h"

typedef struct CanvasState {
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

	c->line_width = line_width; 
	c->draw_color = draw_color;

	c->current_group = NULL;
	c->prev_x = -1;
	c->prev_y = -1;

	return c;
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
		goo_canvas_polyline_new_line(state->current_group,
																state->prev_x, state->prev_y,
																x2, y2,
																NULL);
	}
}

static void canvas_cb_unref(GtkWidget* _widget, gpointer data_canvas_state) {
	free(data_canvas_state);
}

static gboolean canvas_cb_button_press_event(GtkWidget* widget,
										GdkEventButton* event,
										gpointer data)
{
	CanvasState* state = CANVAS_STATE(data);
	if (event->button == GDK_BUTTON_PRIMARY) {
		canvas_state_current_drawing_new(state, event->x, event->y);
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

static gboolean canvas_cb_key_press(GtkWidget* widget, GdkEventKey* event, gpointer data) {
	CanvasState* state = CANVAS_STATE(data);

	// TODO Create stack based history redo/undo
	
	// undo last stroke 
	if (event->keyval == GDK_KEY_R) {
		int len = goo_canvas_item_get_n_children(state->root);
		if (len > 0) goo_canvas_item_remove_child(state->root, len-1);
	}

	// clear canvas
	if (event->keyval == GDK_KEY_C) {
		int len = goo_canvas_item_get_n_children(state->root);
		while(len > 0){
			goo_canvas_item_remove_child(state->root, len-1);
			len--;
		}
	}

	// select colors
	if (event->keyval == GDK_KEY_1) gdk_rgba_parse(state->draw_color, COLOR_1);
	if (event->keyval == GDK_KEY_2) gdk_rgba_parse(state->draw_color, COLOR_2);
	if (event->keyval == GDK_KEY_3) gdk_rgba_parse(state->draw_color, COLOR_3);
	if (event->keyval == GDK_KEY_4) gdk_rgba_parse(state->draw_color, COLOR_4);
	if (event->keyval == GDK_KEY_e) gdk_rgba_parse(state->draw_color, BACKGROUND_COLOR);


	// change line width
	if (event->keyval == GDK_KEY_a && (*state->line_width + LINE_VARY) < MAX_LINE_WIDTH) {
		*state->line_width += LINE_VARY;
	} 

	if (event->keyval == GDK_KEY_s && (*state->line_width + LINE_VARY) > MIN_LINE_WIDTH) {
		*state->line_width -= LINE_VARY;
	} 

	if (event->keyval == GDK_KEY_d) *state->line_width = LINE_WIDTH;
	

	return TRUE;
}

static void canvas_cb_on_resize(GtkWidget* widget, GdkRectangle* allocation, gpointer canvas) {
	int width, height = 0;
	gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
	goo_canvas_set_bounds(GOO_CANVAS(canvas), 0, 0, width, height);
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
void canvas_init(GtkWidget* container, GdkRGBA* draw_color, GdkRGBA* bg_color, double* line_width) {
	GtkWidget* canvas = goo_canvas_new();
	CanvasState* canvas_state = canvas_state_new(canvas, draw_color, line_width);
	g_object_set(G_OBJECT(canvas), "background-color-gdk-rgba", bg_color, NULL);
	

	gtk_container_add(GTK_CONTAINER(container), canvas);

	gtk_widget_set_events(canvas, gtk_widget_get_events(canvas)
						  | GDK_BUTTON_PRESS_MASK
						  | GDK_POINTER_MOTION_MASK
						  | GDK_STRUCTURE_MASK);

	g_signal_connect(canvas, "button-press-event", G_CALLBACK(canvas_cb_button_press_event), canvas_state);
	g_signal_connect(canvas, "button-release-event", G_CALLBACK(canvas_cb_button_release_event), canvas_state);
	g_signal_connect(canvas, "motion-notify-event", G_CALLBACK(canvas_cb_motion_notify_event), canvas_state);

	GtkWidget* window = gtk_widget_get_toplevel(canvas);
	if (!GTK_IS_WINDOW(window)) {
		g_printerr("ERROR: toplevel of canvas is not window \n%s:%d\n", __FILE__, __LINE__);
		exit(2);
	}
	g_signal_connect(window, "destroy", G_CALLBACK(canvas_cb_unref), canvas_state);
	g_signal_connect(window, "size-allocate", G_CALLBACK(canvas_cb_on_resize), canvas);
	g_signal_connect(window, "key-press-event", G_CALLBACK(canvas_cb_key_press), canvas_state);
}
