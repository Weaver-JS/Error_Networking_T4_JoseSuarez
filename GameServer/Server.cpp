
#include "Server.h"


/* 
	Threads se encargan de tener la conexión o de enviar y recibir.
*/




Server::Server() :
	commandLineThread(&Server::throwCommandLine, this)
{
	
	
	ip = sf::IpAddress::getLocalAddress();
	newMessage = false;
	connectionOn = true;
	commandLineThread.launch();
	doOnce = false;
}


Server::~Server()
{
	//closeConne{
	listener.close();
}

void Server::initServer()
{
	
	std::cout << "Server working correctly" << std::endl;
	while (connectionOn)
	{
		if (socket_list.size()  < 2)
		{
			listener.setBlocking(true);
			sf::Socket::Status status = listener.listen(REGULARPORT);
			if (status != sf::Socket::Done)
			{
				//std::cout << "Imposible vincular al puerto " << REGULARPORT << std::endl;
			}
			else if (status == sf::Socket::Done)
			{

				std::cout << "Vinculado correctamente al puerto " << REGULARPORT << std::endl;



			}


			sf::TcpSocket* p;
			p = new sf::TcpSocket();

			if (listener.accept(*p) != sf::Socket::Done)
			{
				delete p;
				std::cout << "No se puede vincular al socket receive" << std::endl;
			}
			else
			{
				socket_list.push_back(p);// = new sf::TcpSocket();
				socket_list.at(socket_list.size() - 1)->setBlocking(true);
				int num = socket_list.size() - 1;



				std::cout << "Socket receive aceptado, en el puerto local:" << socket_list.at(num)->getLocalPort() << " Con la dirección " << socket_list.at(num)->getRemoteAddress() << "Y con el puerto remoto: " << socket_list.at(num)->getRemotePort() << std::endl;
				socket_list.at(num)->setBlocking(false);




			}

		}
		else if (socket_list.size() == 2 && doOnce == false)
		{
			state = gameStates::ASKNAMES;
			sf::String ms = "start";
			send(ms);
			doOnce = true;
		}
		else if(doOnce)
		{
			sf::sleep(sf::seconds(2000.0f));
		}
	}
}







void Server::recv()
{
	while (connectionOn)
	{
		switch (state)
		{
		case CONNECT:
			
			break;
		case ASKNAMES:
			recv_names();
			break;
		case PLAY:
			break;
		default:
			break;
		}

	}
}





void Server::recv_names()
{
	

		for (int i = 0; i < socket_list.size(); i++)
		{
			char data[1300];
			std::size_t bytesReceived;
			sf::Socket::Status status = socket_list.at(i)->receive(data, sizeof(data), bytesReceived);

			switch (status)
			{
			case sf::Socket::Error:
			case sf::Socket::Disconnected:
				std::cout << "Fallo de conexión, no llegan los paquetes" << std::endl;
				if (connectionOn)
					closeConnections(i);
				break;
			case sf::Socket::Partial:
				std::cout << "ESTADO PARCIAL" << std::endl;
				break;

			case sf::Socket::Done:
				if (bytesReceived > 0)
				{

					messageRecieved = data;
					sf::String message = messageRecieved;
					if (i == 0)
					{
						//player_1.setMessageInfo(messageRecieved);
						//player_1.getMessageType();
						//player_1.setNames(messageRecieved);
						std::string messageToPlayer = PLAYER_2 + message;
						message = PLAYER_1   + message;
						sendToPlayer1(message);
						sendToplayer2(messageToPlayer);
					}
					else
					{

						//player_2.setMessageInfo(messageRecieved);
						//player_2.getMessageType();
						//player_2.setNames(messageRecieved);
						std::string messageToPlayer = PLAYER_2 + message;
						message = PLAYER_1 + message;
						sendToPlayer1(messageToPlayer);
						sendToplayer2(message);
					}




					//send(message);


				}
				break;
			default:
				//	std::cout << "Se te olvido un estado, gilipollas. Es este para que no busques más" << status << " Gilipollas" << std::endl;
				break;

			}
		}
		sf::sleep(sf::milliseconds(200.0f));
	
}

void Server::send(sf::String & ms)
{
	sendMessage = ms;

	for (int i = 0; i < socket_list.size(); i++)
	{
		if (sendMessage.length() > 0)
		{
			size_t sentBytes;
			sf::Socket::Status status = socket_list.at(i)->send(sendMessage.c_str(), sendMessage.length() + 1, sentBytes);
			
			
				if (status != sf::Socket::Done && status != sf::Socket::Partial)
				{
					std::cout << "Wrong connection, problem sending packages" << std::endl;
					if(connectionOn)
					closeConnections(i);
				}
				else if (status == sf::Socket::Partial)
				{
					sf::String ms = sendMessage.substr(sendMessage.at(sentBytes), sendMessage.size());
					send(ms);
				}
				
			
		}
	}
}

void Server::closeConnections()
{
	sf::String s = "exit";
	send(s);
	connectionOn = false;
	for (int i = 0; i < socket_list.size(); i++)
	{
		socket_list.at(i)->disconnect();
		
		delete socket_list.at(i);
		socket_list.erase(socket_list.begin() + i);
	}

	
	
	
	commandLineThread.terminate();

}

void Server::closeConnections(int coord)
{
	std::cout << "User :" << socket_list.at(coord)->getRemoteAddress() << "With local port:" << socket_list.at(coord)->getLocalPort() << "And remote port " << socket_list.at(coord)->getRemotePort() << " Array coordinate:" << coord << std::endl;
	socket_list.at(coord)->disconnect();
	delete socket_list.at(coord);
	socket_list.erase(socket_list.begin() + coord);

	
	
	
}

bool Server::isConnected()
{
	return connectionOn;
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

void Server::throwCommandLine()
{
	std::string command;
	while (connectionOn)
	{
		
		
		std::cin >> command;
		
		
		switch (str2int(command.c_str()))
		{
		case str2int("exit"):
			closeConnections();
			break;

		case str2int("EXIT"):
			closeConnections();
			break;
		default:
			std::cout << "Wrong Input, available inputs: 'exit' 'EXIT'" << std::endl;
			break;
		}
	}
	commandLineThread.terminate();
}

void Server::sendToPlayer1(const sf::String & ms)
{
	sendMessage = ms;


		if (sendMessage.length() > 0)
		{
			size_t sentBytes;
			sf::Socket::Status status = socket_list.at(0)->send(sendMessage.c_str(), sendMessage.length() + 1, sentBytes);


			if (status != sf::Socket::Done && status != sf::Socket::Partial)
			{
				std::cout << "Wrong connection, problem sending packages" << std::endl;
				if (connectionOn)
					closeConnections(0);
			}
			else if (status == sf::Socket::Partial)
			{
				sf::String ms = sendMessage.substr(sendMessage.at(sentBytes), sendMessage.size());
				sendToPlayer1(ms);
			}


		}
	
}

void Server::sendToplayer2(const sf::String & ms)
{
	sendMessage = ms;


	if (sendMessage.length() > 0)
	{
		size_t sentBytes;
		sf::Socket::Status status = socket_list.at(1)->send(sendMessage.c_str(), sendMessage.length() + 1, sentBytes);


		if (status != sf::Socket::Done && status != sf::Socket::Partial)
		{
			std::cout << "Wrong connection, problem sending packages" << std::endl;
			if (connectionOn)
				closeConnections(1);
		}
		else if (status == sf::Socket::Partial)
		{
			sf::String ms = sendMessage.substr(sendMessage.at(sentBytes), sendMessage.size());
			sendToplayer2(ms);
		}


	}
}

std::string & Server::getMessageRecieved()
{
	return messageRecieved;
}


