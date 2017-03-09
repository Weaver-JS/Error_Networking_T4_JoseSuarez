#include "Server.h"


/* 
	Threads se encargan de tener la conexión o de enviar y recibir.
*/




Server::Server()
{
	ip = sf::IpAddress::getLocalAddress();
	newMessage = false;
	connectionOn = true;
	state = CONNECT;
	myName = "";
	enemyName = "";
	myMessage = "";
	enemyMessage = "";
}


Server::~Server()
{
}

gameStates & Server::getState()
{
	return state;
}

void Server::initClient()
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



void Server::recv()
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
			break;
		default:
			break;
		}


		//main_socket.setBlocking(false);

	}
}

void Server::recv_names()
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
					protocol = getProtocol(messageRecieved);
				if (myName == "" &&  protocol == "PLAYER1_")
				{
					
					myName = messageRecieved;
					myMessage = messageRecieved;
				}
				else if (enemyName == "" && protocol == "PLAYER2_")
				{
					enemyName = messageRecieved;
					enemyMessage = messageRecieved;
				}
				
				else if (myName != "" && myName != "")
				{
					state = gameStates::PLAY;
				}
				
					
				
			}
		
			//	std::cout << "Se te olvido un estado, gilipollas. Es este para que no busques más"<<status<< " Gilipollas" << std::endl;
			break;
		}
		sf::sleep(sf::milliseconds(200.0f));
}

void Server::recv_gameUp()
{
	char data[1300];
	std::size_t bytesReceived;
	//socket_recv.connect(IP, REGULARPORT);


	//main_socket.setBlocking(true);
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
	sf::sleep(sf::milliseconds(200.0f));
}	


void Server::send(sf::String & ms)
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
				sf::String ms = sendMessage.substr(sendMessage.at(sentBytes), sendMessage.size());
				send(ms);
			}
		}
	}

void Server::closeConnections()
	{
		connectionOn = false;

		main_socket.disconnect();


	}

	bool Server::isConnected()
	{
		return connectionOn;
	}

	std::string & Server::getMessageRecieved()
	{
		return messageRecieved;
	}

	std::string Server::getProtocol(std::string & completeMessage)
	{
		std::string messageType;
		//messageType.resize(completeMessage.size());
		int length_identificator = 0;
		if (completeMessage.size() > 3)
		{
			for (int i = 0; i < completeMessage.size(); i++)
			{
				if (i > 0 && completeMessage[i - 1] == '_')
				{
					length_identificator = i;
					break;
				}
				else
					messageType.push_back(completeMessage[i]);

			}
			completeMessage = completeMessage.substr(length_identificator, completeMessage.size() -1);
			return messageType;
		}
		return completeMessage;

	}

	std::string Server::getEnemyName()
	{
		return enemyName;

	}
	std::string  Server::getName()
	{
		return myName;
	}
	
	std::string & Server::getMymessage()
	{
		return myMessage;
	}

	std::string & Server::getEnemyMessage()
	{
		return enemyMessage;
	}

	void Server::setMyMessage(const std::string & ms)
	{
		myMessage = ms;
	}
	void Server::setEnemyMessage(const std::string & ms)
	{
		myMessage = ms;
	}