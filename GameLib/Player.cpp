#include "Player.h"



Player::Player()
{
	
	timer = 0;
	puntuation = 0;
	enemyPuntuation = 0;
	messageInfo = "";
	name = "";
	enemyName = "";
	actualWord = "Escribe tu nombre para empezar la partida.";
	win = false;
	lose = false;
}


constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

 

void Player::setMessageInfo(const std::string & ms)
{
	messageInfo = ms;
}

void Player::setName(const std::string & ms)
{
	if(name == "")
	name = ms;
}

void Player::setEnemyName(const std::string & ms)
{
	if(enemyName == "")
	enemyName = ms;
}

void Player::setPuntuation(const int p)
{
	puntuation = p;
}

void Player::setEnemyPuntuation(const int p)
{
	enemyPuntuation = p;
}

const std::string & Player::getName()
{
	return name;
}

const std::string & Player::getEnemyName()
{
	return enemyName;
}

const int Player::getPuntuation()
{
	return puntuation;
}

const int Player::getEnemyPuntuation()
{
	return enemyPuntuation;
}

bool Player::getWin()
{
	return win;
}

bool Player::getLose()
{
	return lose;
}




std::string Player::getProtocolTag(std::string & completeMessage)
{
	std::string messageType;
	int length_identificator = 0;
	
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
		completeMessage = completeMessage.substr(length_identificator, completeMessage.size() - 1);
		return messageType;
	
	

}

std::string Player::setProtocolTag(std::string & messageToSend, const std::string & constTag, const std::string & destination)
{
	messageToSend = destination + constTag + messageToSend;
	return messageToSend;
}

void Player::protocolManagement(std::string & completeMessage)
{
	std::string protocolTag = getProtocolTag(completeMessage);
	const int name = str2int(NAME);
	const int puntuationC = str2int(PUNTUATION);
	const int word = str2int(dWORD);
	const int success = str2int(SUCCESS);
	const int failed = str2int(FAILED);

	if (protocolTag == PLAYER_1)
	{
		protocolTag = getProtocolTag(completeMessage);
		myMessage = completeMessage;
		switch (str2int(protocolTag.c_str()))
		{
		case name:
			setName(completeMessage);
			break;
		case puntuationC:
			puntuation++;
			break;
		case word:
			actualWord = completeMessage;
			break;
		case success:
			win = true;
			break;
		case failed:
			lose = true;
			break;

		default:
			break;
		}
	}
	else if (protocolTag == PLAYER_2)
	{
		protocolTag = getProtocolTag(completeMessage);
		enemyMessage = completeMessage;
		switch (str2int(protocolTag.c_str()))
		{
	
		case name:
			setEnemyName(completeMessage);
			break;
		case puntuationC:
			enemyPuntuation++;
			break;
		case success:
			enemyWin = true;
			break;
		case failed:
			enemyLose = true;
			break;

		default:
			break;
		}
	}
}

std::string & Player::getMyMessage()
{
	return myMessage;
}

std::string & Player::getEnemyMessage()
{
	return enemyMessage;
}

void Player::setMyMessage(const std::string & ms)
{
	myMessage = ms;
}

void Player::setEnemyMessage(const std::string & ms)
{
	enemyName = ms;
}

std::string & Player::getactualWord()
{
	return actualWord;
}
