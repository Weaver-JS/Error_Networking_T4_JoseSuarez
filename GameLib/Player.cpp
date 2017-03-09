#include "Player.h"



Player::Player()
{
	
	timer = 0;
	puntuation = 0;
	
	messageInfo = "";
	name = "";
	enemyName = "";
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




std::string Player::getProtocolTag(std::string & completeMessage)
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
		completeMessage = completeMessage.substr(length_identificator, completeMessage.size() - 1);
		return messageType;
	}
	return completeMessage;

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
	const int puntuation = str2int(PUNTUATION);
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
		case puntuation:
			setPuntuation(str2int(completeMessage.c_str()));
			break;
		case word:
			break;
		case success:
			break;
		case failed:
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
		case puntuation:
			setPuntuation(str2int(completeMessage.c_str()));
			break;
		case word:
			break;
		case success:
			break;
		case failed:
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

