#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include "Constants.h"
#include <iostream>
#include <list>
#include "Player.h"

enum gameStates {

	CONNECT,
	ASKNAMES,
	PLAY

};
#define NUMOFUSERS 3
class Server
{
	/*
	Si en send() == Sf::socket::partial

	send(char* , size, sizeSent)
	*/
private:
	


	
	sf::TcpListener listener;
	sf::IpAddress ip;
	sf::Thread commandLineThread;
	std::vector<sf::TcpSocket*> socket_list;



	gameStates state;

	std::string messageRecieved;
	std::string sendMessage;
	bool newMessage;
	bool connectionOn;
	bool startGame;
	bool doOnce;
	
public:
	Server();
	~Server();
	void initServer();


	int getTheFirstConnectionGap();

	void recv();
	

	void recv_names();

	void send(sf::String & ms);
	void closeConnections();
	void closeConnections(int coord);
	bool isConnected();
	void throwCommandLine();

	void sendToPlayer1(const sf::String & ms);
	void sendToplayer2(const sf::String & ms);
	

	std::string & getMessageRecieved();
};

