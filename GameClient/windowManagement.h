
#pragma once
#include <SFML\Graphics.hpp>
#include "Client.h"
#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>

enum gameState
{
	WAITINGOTHERPLAYER,
	TYPEYOURNAME,
	GAME
};

class windowManagement
{
private:
	std::vector<std::string> aMensajes;
	std::vector<std::string> enemyMessages;
	sf::Vector2i screenDimensions;
	sf::RenderWindow windowRenderer;
	sf::Font font;
	sf::String mensaje;
	sf::Text chattingText;
	sf::Text enemyChattingText;
	sf::Text text;
	sf::Text timer;
	sf::Text word;

	sf::Text textName;
	sf::Text enemyName;

	sf::RectangleShape separator;
	sf::RectangleShape separatorup;

	sf::RectangleShape nameUpseparator;
	sf::RectangleShape namedownSeparator;

	sf::RectangleShape puntuationSeparator;
	Client serverOrClient;
	

	windowManagement() {}
	~windowManagement();

public:
	static windowManagement& get() {
		static windowManagement instance;
		return instance;
	}
	windowManagement(windowManagement const&) = delete;
	void operator=(windowManagement const&) = delete;

	void init(float x, float y, std::string chatName);
	void loop();
	void windowStuff(sf::Event & evento);
	sf::RenderWindow & getWindowRenderer();
};

