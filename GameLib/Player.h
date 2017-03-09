#pragma once
#include <iostream>

#define NAME "NAME_"
#define PUNTUATION "PUNT_"
#define dWORD "WORD_"
#define WIN "WIN_" //se envía un 0 o 1 si ha ganado WL_1
#define LOSE "LOSE_"
#define TIMER_START "START_"
#define TIMER_END "END_"
#define SUCCESS "SUCCESS_"
#define FAILED "FAILED_"
#define PLAYER_1 "PLAYER1_"
#define PLAYER_2 "PLAYER2_"



 class Player
{
private:
	std::string name;
	std::string enemyName;
	int puntuation;
	int timer;
	
	std::string myMessage;
	std::string enemyMessage;
	
	std::string messageInfo;
	std::string word;
	bool win;
	bool lose;
	
	
public:
	
	Player();
	void setNames(const std::string & ms);
	void readMessage(const std::string & ms);
	void setMessageInfo(const std::string & ms);

	void setName(const std::string & ms);
	void setEnemyName(const std::string & ms);
	void setPuntuation(const int p);
	void setWin();
	void setLose();

	const std::string & getName();
	const std::string & getEnemyName();
	const int getPuntuation();
	bool getWin();
	bool getLose();

	std::string getProtocolTag(std::string & completeMessage);
	std::string setProtocolTag(std::string & messageToSend, const std::string & constTag, const std::string & destination);
	void protocolManagement(std::string & completeMessage);

	std::string & getMyMessage();
	std::string & getEnemyMessage();
	void setMyMessage(const std::string & ms);
	void setEnemyMessage(const std::string & ms);


};

