#include <stdlib.h>
#include <gtk/gtk.h>

#include "window.h"
#include "config.h"
#include "canvas.h"


// gboolean blub(GtkWidget* widget, GdkEventButton* event, gpointer data) {
// 	gboolean is_visible = gtk_widget_is_visible(GTK_WIDGET(data));
// 	gtk_widget_set_visible(GTK_WIDGET(data), !is_visible);
// 	return TRUE;
// }

// gboolean overlay_cb_get_child_position(GtkOverlay* overlay, GtkWidget* widget,
// 									   GdkRectangle* allocation, gpointer data)
// {
// 	if(GTK_IS_BOX(widget)) {
// 		allocation->x = 10;
// 		allocation->y = 100;
// 		allocation->height = 500;
// 		allocation->width = 80;
// 	} else {
// 		return FALSE;
// 	}
// 	return TRUE;
// }

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

	app_state->selected_line_width = LINE_WIDTH;
	
	return app_state;
}

static void color_button_cb_color_set(GtkColorButton* widget, gpointer data) {
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), (GdkRGBA*) data);
}

static void window_cb_destroy(GtkWidget* widget, gpointer app_state) {
	free(app_state);
}

void window_init(GtkWidget* window) {

	gtk_window_set_title(GTK_WINDOW(window), "kritzel - Drawing application");
	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

	GtkWidget* overlay = gtk_overlay_new();
	gtk_container_add(GTK_CONTAINER(window), overlay);

	AppState* app_state = app_state_new();
	canvas_init(overlay, &app_state->selected_color, &app_state->background_color, &app_state->selected_line_width);

	GtkWidget* color_button = gtk_color_button_new_with_rgba(&app_state->selected_color);
	gtk_widget_set_size_request(color_button, 50, 50);
	gtk_widget_set_halign(color_button, GTK_ALIGN_START);
	gtk_widget_set_valign(color_button, GTK_ALIGN_START);

	GtkCssProvider* provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider,
									"button {margin: 10px; border: transparent; background: none}\0", -1, NULL);
	gtk_style_context_add_provider(gtk_widget_get_style_context(color_button),
								   GTK_STYLE_PROVIDER(provider),
								   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_style_context_add_class(gtk_widget_get_style_context(color_button), "circular");



	g_signal_connect(color_button, "color-set", G_CALLBACK(color_button_cb_color_set), &app_state->selected_color);
	gtk_overlay_add_overlay(GTK_OVERLAY(overlay), color_button);

	g_signal_connect(window, "destroy", G_CALLBACK(window_cb_destroy), app_state);


	gtk_widget_show_all(window);
}