#include <gtk/gtk.h>
#include <goocanvas.h>

#include "canvas.h"

gboolean blub(GtkWidget* widget, GdkEventButton* event, gpointer data) {
	gboolean is_visible = gtk_widget_is_visible(GTK_WIDGET(data));
	gtk_widget_set_visible(GTK_WIDGET(data), !is_visible);
	return TRUE;
}

gboolean overlay_cb_get_child_position(GtkOverlay* overlay, GtkWidget* widget,
									   GdkRectangle* allocation, gpointer data)
{
	if(GTK_IS_BOX(widget)) {
		g_print("Box\n ");
		allocation->x = 10;
		allocation->y = 100;
		allocation->height = 500;
		allocation->width = 80;
	} else {
		g_print("Not Box\n");
		return FALSE;
	}
	return TRUE;
}

/*
  called when application is started without command line arguments
*/
static void activate(GtkApplication *app, gpointer _user_data) {
	GtkWidget* window = gtk_application_window_new(app);

	gtk_window_set_title(GTK_WINDOW(window), "kritzel - Drawing application");
	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
	GtkWidget* overlay = gtk_overlay_new();

	gtk_container_add(GTK_CONTAINER(window), overlay);


	canvas_init(overlay);
	GtkWidget* button = gtk_button_new();
	gtk_widget_set_size_request(button, 50, 50);

	GtkCssProvider* provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider,
									"button {margin: 10px}\0", -1, NULL);
	gtk_style_context_add_provider(gtk_widget_get_style_context(button),
								   GTK_STYLE_PROVIDER(provider),
								   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


	gtk_widget_set_halign(button, GTK_ALIGN_START);
	gtk_widget_set_valign(button, GTK_ALIGN_START);
	gtk_style_context_add_class(gtk_widget_get_style_context(button), "circular");


	GtkWidget* ui_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkCssProvider* ui_box_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(ui_box_provider,
									"box {background-color: rgba(0, 0, 0, 0.1); padding-top: 30px; padding-bottom: 30px; padding-right: 10px; padding-left: 10px; border-radius: 20px}\n", -1, NULL);


	gtk_style_context_add_provider(gtk_widget_get_style_context(ui_box),
								   GTK_STYLE_PROVIDER(ui_box_provider),
								   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	GtkWidget* color_button = gtk_color_button_new();

	gtk_style_context_add_class(gtk_widget_get_style_context(color_button), "circular");
	gtk_container_add(GTK_CONTAINER(ui_box), color_button);

	g_signal_connect(button, "button-press-event", G_CALLBACK(blub), ui_box);
	g_signal_connect(overlay, "get-child-position", G_CALLBACK(overlay_cb_get_child_position), NULL);

	gtk_overlay_add_overlay(GTK_OVERLAY(overlay), button);
	gtk_overlay_add_overlay(GTK_OVERLAY(overlay), ui_box);


	gtk_widget_show_all(window);
	gtk_widget_hide(ui_box);
}


int main(int argc, char* argv[]) {

	GtkApplication* app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return status;
}
