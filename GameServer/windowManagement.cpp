#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	
	std::cout << "Server init" << std::endl;


	

}

void windowManagement::loop()
{
	sf::Thread server_listen(&Client::initServer, &serverOrClient);
	
	server_listen.launch();
	
		
	serverOrClient.recv();
	
	

	
	server_listen.terminate();
//	serverOrClient.closeConnections();
//	recv_thread.terminate();
	
	

}




sf::RenderWindow & windowManagement::getWindowRenderer()
{
	
	return get().windowRenderer;
}


windowManagement::~windowManagement()
{
}
