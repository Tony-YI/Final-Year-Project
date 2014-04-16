#include "GUI.h"


GtkWidget* GUI::window = NULL;
GtkWidget* GUI::button = NULL;
GtkWidget* GUI::table = NULL;
GtkWidget* GUI::label = NULL;
struct GUI_quad_status_frame GUI::quad_status_frame[QUAD_NB + 1];
struct GUI_quad_control_panel GUI::quad_control_panel;
struct GUI_quad_flight_control GUI::quad_flight_control;

GUI::GUI(int argc, char** argv)
{
	#ifdef QUAD_NB
	printf("Quadcopter number is %d\n",QUAD_NB);
	#endif

	table = gtk_table_new(2,QUAD_NB + 1,TRUE);
	gtk_init(&argc,&argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Quadcopter Ground Control Center");
	g_signal_connect(window, "delete_event", G_CALLBACK(delete_event),NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	gtk_container_add(GTK_CONTAINER(window),table);

	//Construction and display of gtk frame that will show control panel
	quad_control_panel = GUI_generate_quad_control_panel();
	GUI_show_quad_control_panel();
	gtk_table_attach_defaults(GTK_TABLE(table),quad_control_panel.frame,0,QUAD_NB,0,1);

	quad_flight_control = GUI_generate_quad_flight_control();
	GUI_show_quad_flight_control();
	gtk_table_attach_defaults(GTK_TABLE(table),quad_flight_control.frame,QUAD_NB,QUAD_NB + 1,0,2);
	//Construction and display of gtk frame that will show quadcopter status.
	for(uint8_t count_ac = 1;count_ac < QUAD_NB + 1;count_ac++)
	{
		quad_status_frame[count_ac] = GUI_generate_quad_status_frame(count_ac);
		GUI_show_quad_status_frame(count_ac);
		gtk_table_attach_defaults(GTK_TABLE(table),quad_status_frame[count_ac].frame,count_ac - 1,count_ac,1,2);
		printf("creating new quad_status_frame done\n");
	}

	gtk_widget_show(table);
	gtk_widget_show(window);
	printf("pre-configuration of GUI is done\n");
}

GUI::~GUI()
{

}

struct GUI_quad_status_frame GUI::GUI_generate_quad_status_frame(uint8_t AC_ID)
{
	struct GUI_quad_status_frame tmp;
	char buffer[32];
	sprintf(buffer,"Quadcopter %d status",AC_ID);
	tmp.frame = gtk_frame_new(buffer);
	tmp.box = gtk_vbox_new(FALSE,0);
	tmp.label_ned_x = gtk_label_new("ned x: 0");
	tmp.label_ned_y = gtk_label_new("ned y: 0");
	tmp.label_ned_z = gtk_label_new("ned z: 0");
	tmp.label_ecef_x = gtk_label_new("ecef x: 0");
	tmp.label_ecef_y = gtk_label_new("ecef y: 0");
	tmp.label_ecef_z = gtk_label_new("ecef z: 0");
	tmp.label_pacc = gtk_label_new("pacc : 0");
	tmp.label_state = gtk_label_new("state: 0");

	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_ned_x,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_ned_y,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_ned_z,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_ecef_x,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_ecef_y,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_ecef_z,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_pacc,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.label_state,TRUE,TRUE,0);

	gtk_container_add(GTK_CONTAINER(tmp.frame),tmp.box);

	return tmp;
}

void GUI::GUI_show_quad_status_frame(uint8_t AC_ID)
{
	gtk_widget_show(quad_status_frame[AC_ID].label_ned_x);
	gtk_widget_show(quad_status_frame[AC_ID].label_ned_y);
	gtk_widget_show(quad_status_frame[AC_ID].label_ned_z);
	gtk_widget_show(quad_status_frame[AC_ID].label_ecef_x);
	gtk_widget_show(quad_status_frame[AC_ID].label_ecef_y);
	gtk_widget_show(quad_status_frame[AC_ID].label_ecef_z);
	gtk_widget_show(quad_status_frame[AC_ID].label_pacc);
	gtk_widget_show(quad_status_frame[AC_ID].label_state);
	gtk_widget_show(quad_status_frame[AC_ID].box);
	gtk_widget_show(quad_status_frame[AC_ID].frame);
	return ;
}

struct GUI_quad_control_panel GUI::GUI_generate_quad_control_panel()
{
	struct GUI_quad_control_panel tmp;
	
	tmp.frame = gtk_frame_new("Quadcopters Control");
	tmp.box = gtk_vbox_new(FALSE,0);
	tmp.button_init = gtk_button_new_with_label("init");
	tmp.button_end_negotiate = gtk_button_new_with_label("end negotiation");
	tmp.button_start_engine = gtk_button_new_with_label("start engine");
	tmp.button_takeoff = gtk_button_new_with_label("takeoff");
	tmp.button_landhere = gtk_button_new_with_label("landhere");
	
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_init,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_end_negotiate,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_start_engine,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_takeoff,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_landhere,TRUE,TRUE,0);

	gtk_container_add(GTK_CONTAINER(tmp.frame),tmp.box);
	
	return tmp;
}

void GUI::GUI_show_quad_control_panel()
{
	gtk_widget_show(quad_control_panel.button_init);
	gtk_widget_show(quad_control_panel.button_end_negotiate);
	gtk_widget_show(quad_control_panel.button_start_engine);
	gtk_widget_show(quad_control_panel.button_takeoff);
	gtk_widget_show(quad_control_panel.button_landhere);
	gtk_widget_show(quad_control_panel.box);
	gtk_widget_show(quad_control_panel.frame);

	return ;
}

struct GUI_quad_flight_control GUI::GUI_generate_quad_flight_control()
{
	struct GUI_quad_flight_control tmp;
	tmp.frame = gtk_frame_new("Flight Control");
	tmp.box = gtk_vbox_new(FALSE,0);
	tmp.button_execute = gtk_button_new_with_label("execute");
	tmp.button_go_north = gtk_button_new_with_label("go north");
	tmp.button_go_south = gtk_button_new_with_label("go south");
	tmp.button_go_west = gtk_button_new_with_label("go west");
	tmp.button_go_east = gtk_button_new_with_label("go east");

	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_execute,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_go_north,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_go_south,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_go_east,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(tmp.box),tmp.button_go_west,TRUE,TRUE,0);
	gtk_container_add(GTK_CONTAINER(tmp.frame),tmp.box);
	
	return tmp;
}

void GUI::GUI_show_quad_flight_control()
{
	gtk_widget_show(quad_flight_control.button_execute);
	gtk_widget_show(quad_flight_control.button_go_north);
	gtk_widget_show(quad_flight_control.button_go_south);
	gtk_widget_show(quad_flight_control.button_go_east);
	gtk_widget_show(quad_flight_control.button_go_west);
	gtk_widget_show(quad_flight_control.box);
	gtk_widget_show(quad_flight_control.frame);
	return ;
}
void GUI::GUI_main()
{
	gtk_main();
}
gboolean GUI::delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_main_quit();
	return FALSE;
}

void GUI::callback(GtkWidget *widget, gpointer data)
{
	g_print("Hello again - %s was presssed\n",(gchar*) data);
	gtk_label_set_text(GTK_LABEL(label),(gchar*) data);	
}

void GUI::button_add_event_listener(GtkWidget *button, void *fun(void *),void * arg)
{
	g_signal_connect(button,"clicked",G_CALLBACK(fun),(gpointer) arg);
	return ;
}
