#include <gtk/gtk.h>
#include "GUI.h"

int main(int argc, char **argv)
{
	GUI graphic(argc,argv);
	graphic.GUI_main();
	return 0;	
}
