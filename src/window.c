#include <gtk/gtk.h>
#include "canvas.h"

#include "window.h"

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
	GdkRGBA* selected_color;
	GdkRGBA* background_color;
	double selected_line_width;

} UiState;

UiState* ui_state_new() {
	UiState* ui_state = malloc(sizeof(UiState));

	ui_state->selected_color = malloc(sizeof(GdkRGBA));
	ui_state->selected_color->red = 0.0;
	ui_state->selected_color->green = 1.0;
	ui_state->selected_color->blue = 1.0;
	ui_state->selected_color->alpha = 1.0;

	ui_state->background_color = malloc(sizeof(GdkRGBA));
	ui_state->background_color->red = 1.0;
	ui_state->background_color->green = 1.0;
	ui_state->background_color->blue = 1.0;
	ui_state->background_color->alpha = 1.0;

	ui_state->selected_line_width = 5.0;
	
	return ui_state;
}

static void color_button_cb_color_set(GtkColorButton* widget, gpointer data) {
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), (GdkRGBA*) data);
}

static void window_cb_destroy(GtkWidget* widget, gpointer ui_state) {
	gdk_rgba_free(((UiState*) ui_state)->selected_color);
	gdk_rgba_free(((UiState*) ui_state)->background_color);
	free(ui_state);
}

void window_init(GtkWidget* window) {

	gtk_window_set_title(GTK_WINDOW(window), "kritzel - Drawing application");
	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);

	GtkWidget* overlay = gtk_overlay_new();
	gtk_container_add(GTK_CONTAINER(window), overlay);

	UiState* ui_state = ui_state_new();
	canvas_init(overlay, ui_state->selected_color, ui_state->background_color, &ui_state->selected_line_width);


	GtkWidget* color_button = gtk_color_button_new_with_rgba(ui_state->selected_color);
	gtk_widget_set_halign(color_button, GTK_ALIGN_START);
	gtk_widget_set_valign(color_button, GTK_ALIGN_START);

	g_signal_connect(color_button, "color-set", G_CALLBACK(color_button_cb_color_set), ui_state->selected_color);
	gtk_overlay_add_overlay(GTK_OVERLAY(overlay), color_button);

	// GtkWidget* button = gtk_button_new();
	// gtk_widget_set_size_request(button, 50, 50);

	// GtkCssProvider* provider = gtk_css_provider_new();
	// gtk_css_provider_load_from_data(provider,
	// 								"button {margin: 10px}\0", -1, NULL);
	// gtk_style_context_add_provider(gtk_widget_get_style_context(button),
	// 							   GTK_STYLE_PROVIDER(provider),
	// 							   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


	// gtk_widget_set_halign(button, GTK_ALIGN_START);
	// gtk_widget_set_valign(button, GTK_ALIGN_START);
	// gtk_style_context_add_class(gtk_widget_get_style_context(button), "circular");


	// GtkWidget* ui_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	// GtkCssProvider* ui_box_provider = gtk_css_provider_new();
	// gtk_css_provider_load_from_data(ui_box_provider,
	// 								"box {background-color: rgba(0, 0, 0, 0.1); padding-top: 30px; padding-bottom: 30px; padding-right: 10px; padding-left: 10px; border-radius: 20px}\n", -1, NULL);


	// gtk_style_context_add_provider(gtk_widget_get_style_context(ui_box),
	// 							   GTK_STYLE_PROVIDER(ui_box_provider),
	// 							   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	// GtkWidget* color_button = gtk_color_button_new();

	// gtk_style_context_add_class(gtk_widget_get_style_context(color_button), "circular");
	// gtk_container_add(GTK_CONTAINER(ui_box), color_button);

	// g_signal_connect(button, "button-press-event", G_CALLBACK(blub), ui_box);
	// g_signal_connect(overlay, "get-child-position", G_CALLBACK(overlay_cb_get_child_position), NULL);
	// g_signal_connect(color_button, "color-set", G_CALLBACK(color_button_cb_color_set), canvas_state);

	// gtk_overlay_add_overlay(GTK_OVERLAY(overlay), button);
	// gtk_overlay_add_overlay(GTK_OVERLAY(overlay), ui_box);
	g_signal_connect(window, "destroy", G_CALLBACK(window_cb_destroy), ui_state);


	gtk_widget_show_all(window);
	//gtk_widget_hide(ui_box);
}