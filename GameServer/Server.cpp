
#include "Server.h"


/* 
	Threads se encargan de tener la conexión o de enviar y recibir.
*/




Client::Client() :
	commandLineThread(&Client::throwCommandLine, this)
{
	
	
	ip = sf::IpAddress::getLocalAddress();
	newMessage = false;
	connectionOn = true;
	commandLineThread.launch();
	doOnce = false;
	wordList.resize(NUMWORDS);
	wordList[0] = "palabra";
	wordList[1] = "libro";
	wordList[2] = "pastilla";
	wordList[3] = "botella";
	wordList[4] = "visual";
	actualWord = -1;
	
}


Client::~Client()
{
	
	listener.close();
}

void Client::initServer()
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
			sf::sleep(sf::seconds(20000.0f));
		}
	}
}







void Client::recv()
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
			recv_gameplay();
			break;
		case ENDGAME:
			closeGame();
			break;
		default:
			break;
		}
		sf::sleep(sf::milliseconds(200.0f));
	}
}





void Client::recv_names()
{
	

		for (int i = 0; i < socket_list.size(); i++)
		{
			char data[1300];
			std::size_t bytesReceived;
			sf::Socket::Status status = socket_list.at(i)->receive(data, sizeof(data), bytesReceived);

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
					sf::String message = messageRecieved;
					if (i == 0)
					{
						
						std::string messageToPlayer = messageRecieved; 
						std::string messageToPlayer2 = messageRecieved; 
						player.setName(messageRecieved);
						player.setProtocolTag(messageToPlayer, NAME, PLAYER_1);
						player.setProtocolTag(messageToPlayer2, NAME, PLAYER_2);
						sendToPlayer1(messageToPlayer);
						sendToplayer2(messageToPlayer2);
					}
					else
					{
						std::string messageToPlayer = messageRecieved;

						std::string messageToPlayer2 = messageRecieved;
						player.setEnemyMessage(messageRecieved);
						player.setProtocolTag(messageToPlayer, NAME, PLAYER_2);
						player.setProtocolTag(messageToPlayer2, NAME, PLAYER_1);

						
						sendToPlayer1(messageToPlayer);
						sendToplayer2(messageToPlayer2);
					}


					if (player.getName() != "" && player.getEnemyName() != "")
					{
						selectNewWord();
						serverTimer.restart();
						state = PLAY;
					}

				}
				break;
			default:
				//	std::cout << "Se te olvido un estado, gilipollas. Es este para que no busques más" << status << " Gilipollas" << std::endl;
				break;

			}
		}
		
	
}

void Client::recv_gameplay()
{
	if (wordList.size() > 0 )
	{
		gameManager();



		for (int i = 0; i < socket_list.size(); i++)
		{
			char data[1300];
			std::size_t bytesReceived;
			sf::Socket::Status status = socket_list.at(i)->receive(data, sizeof(data), bytesReceived);

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
					sf::String message = messageRecieved;
					if (i == 0)
					{

						std::string messageToPlayer = messageRecieved;
						std::string messageToPlayer2 = messageRecieved;
						if (messageToPlayer == ">" + wordList[actualWord])
						{
							int punt = player.getPuntuation();
							punt++;
							player.setPuntuation(punt);
							std::string puntuationStringP1 = std::to_string(punt);
							std::string puntuationStringP2 = puntuationStringP1;
							player.setProtocolTag(puntuationStringP1, PUNTUATION, PLAYER_1);
							player.setProtocolTag(puntuationStringP2, PUNTUATION, PLAYER_2);

							sendToPlayer1(puntuationStringP1);
							sendToplayer2(puntuationStringP2);
							//Call restartGameFunc
							restartGame();

						}
						else
						{
							sf::String ms = messageRecieved;
							std::string message = messageRecieved;
						
							player.setProtocolTag(messageRecieved, REGULAR_WORD, PLAYER_1);
							player.setProtocolTag(message, REGULAR_WORD, PLAYER_2);
							sf::String msg = message;
							sendToPlayer1(ms);
							sendToplayer2(msg);
							
							
							//send(ms);
						}

						
					}
					else
					{

						std::string messageToPlayer = messageRecieved;
						if (messageToPlayer == ">" + wordList[actualWord])
						{
							int punt = player.getEnemyPuntuation();
							punt++;
							player.setPuntuation(punt);
							std::string puntuationStringP1 = std::to_string(punt);
							std::string puntuationStringP2 = puntuationStringP1;
							player.setProtocolTag(puntuationStringP1, PUNTUATION, PLAYER_1);
							player.setProtocolTag(puntuationStringP2, PUNTUATION, PLAYER_2);

							sendToPlayer1(puntuationStringP2);
							sendToplayer2(puntuationStringP1);

							//Call restartGameFunc
							restartGame();
						}
						else
						{
							sf::String ms = messageRecieved;
							std::string message = messageRecieved;

							player.setProtocolTag(messageRecieved, REGULAR_WORD, PLAYER_2);
							player.setProtocolTag(message, REGULAR_WORD, PLAYER_1);
							sf::String msg = message;
							sendToPlayer1(ms);
							sendToplayer2(msg);
						}
					
					
					}




				}
				break;
			default:
				//	std::cout << "Se te olvido un estado, gilipollas. Es este para que no busques más" << status << " Gilipollas" << std::endl;
				break;

			}
		}
	}
	else
	{
		if (player.getPuntuation() > player.getEnemyPuntuation())
		{
			std::string messageToPlayer1 = "";
			player.setProtocolTag(messageToPlayer1, SUCCESS, PLAYER_1);
			std::string messageToPlayer2 = "";
			player.setProtocolTag(messageToPlayer2, FAILED, PLAYER_1);
			sendToPlayer1(messageToPlayer1);
			sendToplayer2(messageToPlayer2);
		}
		else
		{
			std::string messageToPlayer1 = "";
			player.setProtocolTag(messageToPlayer1, SUCCESS, PLAYER_1);
			std::string messageToPlayer2 = "";
			player.setProtocolTag(messageToPlayer2, FAILED, PLAYER_1);
			sendToPlayer1(messageToPlayer2);
			sendToplayer2(messageToPlayer1);
		}
		state = ENDGAME;
	}
	

}

void Client::send(sf::String & ms)
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
					sf::String ms = sendMessage.substr(sentBytes, sendMessage.size());
					send(ms);
				}
				
			
		}
	}
}

void Client::closeConnections()
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

void Client::closeConnections(int coord)
{
	std::cout << "User :" << socket_list.at(coord)->getRemoteAddress() << "With local port:" << socket_list.at(coord)->getLocalPort() << "And remote port " << socket_list.at(coord)->getRemotePort() << " Array coordinate:" << coord << std::endl;
	socket_list.at(coord)->disconnect();
	delete socket_list.at(coord);
	socket_list.erase(socket_list.begin() + coord);

	
	
	
}

bool Client::isConnected()
{
	return connectionOn;
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

void Client::throwCommandLine()
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

void Client::restartGame()
{
	if(wordList.size() > 0)
	{ 
		if (selectNewWord() != "FALLO")
		{
			std::cout << wordList[actualWord] << std::endl;
			sf::String ms = "restart";
			send(ms);
			sf::sleep(sf::milliseconds(100.0f));
			std::string word = wordList[actualWord];
			word = player.setProtocolTag(word, dWORD, PLAYER_1);
			ms = word;
			send(ms);
			/*sendToPlayer1(word);
			sendToplayer2(word);*/
			serverTimer.restart();
		}
		else
		{
			wordList.erase(wordList.begin());
		}
}

}

void Client::closeGame()
{
	if (serverTimer.getElapsedTime() > sf::seconds(10.0))
	{
		closeConnections();
	}
}

void Client::sendToPlayer1(const sf::String & ms)
{
	sendMessage = ms;


		if (sendMessage.length() > 0)
		{
			size_t sentBytes;
			

			sf::Socket::Status status = socket_list[0]->send(sendMessage.c_str(), sendMessage.length() + 1, sentBytes);


			if (status != sf::Socket::Done && status != sf::Socket::Partial)
			{
				std::cout << "Wrong connection, problem sending packages" << std::endl;
				if (!connectionOn)
					closeConnections(0);
			}
			else if (status == sf::Socket::Partial)
			{
				sf::String ms = sendMessage.substr(sentBytes, sendMessage.size());
				sendToPlayer1(ms);
			}


		}
	
}

void Client::sendToplayer2(const sf::String & ms)
{
	sendMessage = ms;


	if (sendMessage.length() > 0)
	{
		size_t sentBytes;
		sf::Socket::Status status = socket_list[1]->send(sendMessage.c_str(), sendMessage.length() + 1, sentBytes);


		if (status != sf::Socket::Done && status != sf::Socket::Partial)
		{
			std::cout << "Wrong connection, problem sending packages" << std::endl;
			if (!connectionOn)
				closeConnections(1);
		}
		else if (status == sf::Socket::Partial)
		{
			sf::String ms = sendMessage.substr(sentBytes, sendMessage.size());
			sendToplayer2(ms);
		}


	}
}

std::string & Client::selectNewWord()
{
	if(actualWord != -1)
	wordList.erase(wordList.begin() + actualWord);

	if (wordList.size() > 0)
	{
		actualWord = rand() % wordList.size();
		//std::cout << wordList[actualWord] << std::endl;
		return wordList[actualWord];
	}
	
	else
	{
		std::string p = "FALLO";
		wordList.push_back(p);
		return wordList[actualWord];
	}
	


}

void Client::gameManager()
{
	if (wordList.size() > 0)
	{
		if (serverTimer.getElapsedTime() > sf::seconds(10.0))
		{
			if (selectNewWord() != "FALLO")
			{
				std::cout << wordList[actualWord] << std::endl;
				sf::String ms = "restart";
				send(ms);
				sf::sleep(sf::milliseconds(100.0f));
				std::string word = wordList[actualWord];
				word = player.setProtocolTag(word, dWORD, PLAYER_1);
				ms = word;
				send(ms);
				/*sendToPlayer1(word);
				sendToplayer2(word);*/
				serverTimer.restart();
			}
			else
			{
				wordList.erase(wordList.begin());
			}

		}
	}
}

std::string & Client::getMessageRecieved()
{
	return messageRecieved;
}


