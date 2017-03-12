#pragma once

#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include "Constants.h"
#include <iostream>
#include <Player.h>
enum gameStates {

	CONNECT,
	ASKNAMES,
	PLAY,
	ENDGAME

};
class Client
{
	/*
	Si en send() == Sf::socket::partial

	send(char* , size, sizeSent)
	*/
private:
	

	
	sf::TcpListener listener;
	sf::IpAddress ip;
	sf::TcpSocket main_socket;
	
	std::string myName;
	std::string enemyName;

	std::string messageRecieved;
	std::string sendMessage;

	std::string enemyMessage;
	std::string myMessage;
	bool newMessage;
	bool connectionOn;
	gameStates state;
	Player player;
	sf::Clock clientClock;
	bool clean;
	
public:
	Client();
	~Client();
	
	void initClient();

	
	void recv();
	void send(sf::String & ms);
	void closeConnections();
	bool isConnected();


	void recv_gameUp();
	void recv_names();
	void recv_gameplay();
	std::string & getMessageRecieved();

	std::string  getProtocol(std::string & completeMessage);
	std::string  getEnemyName();
	std::string  getName();

	std::string & getMymessage();
	std::string & getEnemyMessage();

	void setMyMessage(const std::string & ms);
	void setEnemyMessage(const std::string & ms);
	gameStates & getState();

	Player & getPlayer();
	float getTime();
	bool & getCleanBool();
	void setCleanBool(bool b);
};

