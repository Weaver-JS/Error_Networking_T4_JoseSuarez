#include "windowManagement.h"

#define MAX_MENSAJES 30

int main()
{
	
	windowManagement::get().init(800,800,"chat");
	windowManagement::get().loop();

	/*server_MainThread.terminate();
	server_RecvThread.terminate();*/
	
	return 0;
}