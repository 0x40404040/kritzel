#include <stdlib.h>
#include <gtk/gtk.h>

#include "window.h"
#include "config.h"
#include "canvas.h"

typedef struct {
	GdkRGBA selected_color;
	GdkRGBA background_color;
	double selected_line_width;
	GdkRGBA color_palette[4]; 

} AppState;

AppState* app_state_new() {
	AppState* app_state = malloc(sizeof(AppState));

	// check if all colors have the correct format
	if (!(
		gdk_rgba_parse(&app_state->selected_color, COLOR_4) &&
		gdk_rgba_parse(&app_state->selected_color, COLOR_3) &&
		gdk_rgba_parse(&app_state->selected_color, COLOR_2) &&
		gdk_rgba_parse(&app_state->selected_color, COLOR_1) &&
		gdk_rgba_parse(&app_state->background_color, BACKGROUND_COLOR)
		)) 
	{
			g_printerr("ERROR: could not parse color palette or background color!\n");
			exit(1);
	}

	
	if(LINE_WIDTH < MIN_LINE_WIDTH || LINE_WIDTH > MAX_LINE_WIDTH || LINE_VARY < 0.1 || LINE_VARY > 5.0) {
			g_printerr("ERROR: LINE_WIDTH/LINE_VARY MAX or MIN values are exceeded.\n");
			exit(1);
	}

	app_state->selected_line_width = LINE_WIDTH;
	
	return app_state;
}

static void window_cb_destroy(GtkWidget* widget, gpointer app_state) {
	free(app_state);
}

void window_init(GtkWidget* window) {

	gtk_window_set_title(GTK_WINDOW(window), "kritzel - Drawing application");
	gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

	GtkWidget* overlay = gtk_overlay_new();
	gtk_container_add(GTK_CONTAINER(window), overlay);

	AppState* app_state = app_state_new();
	canvas_init(overlay, &app_state->selected_color, &app_state->background_color, &app_state->selected_line_width);

	g_signal_connect(window, "destroy", G_CALLBACK(window_cb_destroy), app_state);


	gtk_widget_show_all(window);
}