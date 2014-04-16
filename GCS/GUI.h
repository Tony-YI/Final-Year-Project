#ifndef _GUI_H
#define _GUI_H
#include <gtk/gtk.h>
#include "GUI_struct.h"
#include "main.h"

class GUI
{
	private:
		static GtkWidget* window;
		static GtkWidget* button;
		
		static GtkWidget* table;
		static GtkWidget* label;	

		
	public:
		static struct GUI_quad_status_frame quad_status_frame[QUAD_NB + 1];
		static struct GUI_quad_control_panel quad_control_panel;
		static struct GUI_quad_flight_control quad_flight_control;

		GUI(int argc, char**argv);
		~GUI();
		
		static void GUI_main();
		static void callback(GtkWidget *widget, gpointer data);
		static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

		static struct GUI_quad_status_frame GUI_generate_quad_status_frame(uint8_t AC_ID);
		static void GUI_show_quad_status_frame(uint8_t AC_ID);

		static struct GUI_quad_control_panel GUI_generate_quad_control_panel();
		static void GUI_show_quad_control_panel();

		static struct GUI_quad_flight_control GUI_generate_quad_flight_control();
		static void GUI_show_quad_flight_control();

		static void button_add_event_listener(GtkWidget* button, void *fun(void *), void * arg);
};

#endif