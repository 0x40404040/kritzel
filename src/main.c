#include <gtk/gtk.h>
#include <goocanvas.h>

#include "window.h"


/*
  called when application is started without command line arguments
*/
static void activate(GtkApplication *app, gpointer _user_data) {
	GtkWidget* window = gtk_application_window_new(app);


	window_init(window);
}

int main(int argc, char* argv[]) {

	GtkApplication* app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return status;
}
