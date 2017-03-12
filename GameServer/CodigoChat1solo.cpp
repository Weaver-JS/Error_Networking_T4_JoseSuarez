#include "windowManagement.h"
#include "Server.h"
#define MAX_MENSAJES 30

int main()
{
	
	windowManagement::get().init(800,800,"chat");
	windowManagement::get().loop();



	return 0;
}