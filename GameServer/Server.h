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
	PLAY,
	ENDGAME

};
#define NUMOFUSERS 3
#define NUMWORDS 5
class Client
{
	
private:
	


	
	sf::TcpListener listener;
	sf::IpAddress ip;
	sf::Thread commandLineThread;
	std::vector<sf::TcpSocket*> socket_list;
	sf::Clock serverTimer;

	Player player;


	gameStates state;

	std::vector<std::string> wordList;
	int actualWord;

	std::string messageRecieved;
	std::string sendMessage;
	bool newMessage;
	bool connectionOn;
	bool startGame;
	bool doOnce;
	
public:
	Client();
	~Client();
	void initServer();


	int getTheFirstConnectionGap();

	void recv();
	void recv_names();
	void recv_gameplay();

	void send(sf::String & ms);
	void closeConnections();
	void closeConnections(int coord);
	bool isConnected();
	void throwCommandLine();

	void restartGame();
	void closeGame();

	void sendToPlayer1(const sf::String & ms);
	void sendToplayer2(const sf::String & ms);

	std::string & selectNewWord();
	void gameManager();
	

	std::string & getMessageRecieved();
};

