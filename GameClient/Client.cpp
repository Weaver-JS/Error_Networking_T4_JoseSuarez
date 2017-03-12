#include "Client.h"


/* 
	Threads se encargan de tener la conexión o de enviar y recibir.
*/




Client::Client()
{
	ip = sf::IpAddress::getLocalAddress();
	newMessage = false;
	connectionOn = true;
	state = CONNECT;
	myName = "";
	enemyName = "";
	myMessage = "";
	enemyMessage = "";
	clean = false;
}


Client::~Client()
{
}

gameStates & Client::getState()
{
	return state;
}

Player & Client::getPlayer()
{
	// TODO: insert return statement here
	return player;
}

float Client::getTime()
{
	return clientClock.getElapsedTime().asSeconds();
}

bool & Client::getCleanBool()
{
	return clean;
}

void Client::setCleanBool(bool b)
{
	clean = b;
}

void Client::initClient()
{

	if (main_socket.connect(IP, REGULARPORT, sf::seconds(10.0f)) != sf::Socket::Done)
	{
		std::cout << "Imposible conectar el socket Receive -Client" << std::endl;
		closeConnections();
	}
	else
	{
		std::cout << "Socket reveive conectado - Client " << std::endl;
	}

	main_socket.setBlocking(false);
}



void Client::recv()
{

	while (connectionOn)
	{
		switch (state)
		{
		case CONNECT:
			recv_gameUp();
			break;
		case ASKNAMES:
			recv_names();
			break;
		case PLAY:
			recv_gameplay();
			break;
		case ENDGAME:
			break;
		default:
			break;
		}
		sf::sleep(sf::milliseconds(200.0f));

	}
}
void Client::recv_gameplay()
{
	char data[1300];
	std::size_t bytesReceived;

	sf::Socket::Status status = main_socket.receive(data, sizeof(data), bytesReceived);
	switch (status)
	{
	case sf::Socket::Error:
		closeConnections();
		break;
	case sf::Socket::Disconnected:
		std::cout << "Fallo de conexión, no llegan los paquetes" << std::endl;
		closeConnections();
		break;
	case sf::Socket::Partial:
		std::cout << "ESTADO PARCIAL" << std::endl;
		break;

	case sf::Socket::Done:
		if (bytesReceived > 0)
		{
			std::string msRecv = data;
			messageRecieved = data;
			if (messageRecieved == "exit" || messageRecieved == "EXIT")
			{
				closeConnections();
			}
			if (messageRecieved == "restart")
			{
				clientClock.restart();
				clean = true;
			}
			if (messageRecieved == "end")
			{
				state = ENDGAME;
			}

		

			if (messageRecieved != "exit" && messageRecieved != "restart")
			{
				player.protocolManagement(msRecv);
			
				
			}




		}

		break;
	}
	//sf::sleep(sf::milliseconds(200.0f));
	messageRecieved.clear();
}
void Client::recv_names()
{
	char data[1300];
		std::size_t bytesReceived;

		sf::Socket::Status status = main_socket.receive(data, sizeof(data), bytesReceived);
		switch (status)
		{
		case sf::Socket::Error:
			closeConnections();
			break;
		case sf::Socket::Disconnected:
			std::cout << "Fallo de conexión, no llegan los paquetes" << std::endl;
			closeConnections();
			break;
		case sf::Socket::Partial:
			std::cout << "ESTADO PARCIAL" << std::endl;
			break;

		case sf::Socket::Done:
			if (bytesReceived > 0)
			{

				messageRecieved = data;
				if (messageRecieved == "exit" || messageRecieved == "EXIT")
				{
					closeConnections();
				}
			
				std::string protocol = "";
				
		
				player.protocolManagement(messageRecieved);
				std::cout << messageRecieved << std::endl;
				
				if (player.getName() != "" && player.getEnemyName() != "")
				{
					state = PLAY;
					player.getactualWord() = "10 segundos para que empiece la partida";
					clientClock.restart();
				}
					
				
			}
		
			break;
		}
	//	sf::sleep(sf::milliseconds(200.0f));
}

void Client::recv_gameUp()
{
	char data[1300];
	std::size_t bytesReceived;
	
	sf::Socket::Status status = main_socket.receive(data, sizeof(data), bytesReceived);
	switch (status)
	{
	case sf::Socket::Error:
		closeConnections();
		break;
	case sf::Socket::Disconnected:
		std::cout << "Fallo de conexión, no llegan los paquetes" << std::endl;
		closeConnections();
		break;
	case sf::Socket::Partial:
		std::cout << "ESTADO PARCIAL" << std::endl;
		break;

	case sf::Socket::Done:
		if (bytesReceived > 0)
		{

			messageRecieved = data;
			if (messageRecieved == "exit" || messageRecieved == "EXIT")
			{
				closeConnections();
			}
			if (messageRecieved == "start")
			{
				state = gameStates::ASKNAMES;
			}

		}
		break;
	default:
		//	std::cout << "Se te olvido un estado, gilipollas. Es este para que no busques más"<<status<< " Gilipollas" << std::endl;
		break;
	}
	messageRecieved = "";
//	sf::sleep(sf::milliseconds(200.0f));
}	


void Client::send(sf::String & ms)
	{
		sendMessage = ms;
		if (sendMessage.length() > 0)
		{
			size_t sentBytes;
			sf::Socket::Status status = main_socket.send(sendMessage.c_str(), sendMessage.length() + 1, sentBytes);
			;
			if (status != sf::Socket::Done && status != sf::Socket::Partial)
			{
				std::cout << "Wrong connection, problem sending packages" << std::endl;
				closeConnections();
			}
			else if (status == sf::Socket::Partial)
			{
				sf::String ms = sendMessage.substr(sentBytes, sendMessage.size());
				send(ms);
			}
		}
	}

void Client::closeConnections()
	{
		connectionOn = false;

		main_socket.disconnect();


	}

	bool Client::isConnected()
	{
		return connectionOn;
	}

	std::string & Client::getMessageRecieved()
	{
		return messageRecieved;
	}



	std::string Client::getEnemyName()
	{
		
		return  player.getEnemyName();

	}
	std::string  Client::getName()
	{
		return player.getName();
	}
	
	std::string & Client::getMymessage()
	{
		return player.getMyMessage();
	}

	std::string & Client::getEnemyMessage()
	{
		return player.getEnemyMessage();
	}

	void Client::setMyMessage(const std::string & ms)
	{
		myMessage = ms;
	}
	void Client::setEnemyMessage(const std::string & ms)
	{
		myMessage = ms;
	}