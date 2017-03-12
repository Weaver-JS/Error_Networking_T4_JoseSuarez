
#pragma once
#include <SFML\Graphics.hpp>
#include "Server.h"
#include <string>
#include <iostream>
#include <vector>




class windowManagement
{
private:
	std::vector<std::string> aMensajes;
	sf::Vector2i screenDimensions;
	sf::RenderWindow windowRenderer;
	sf::Font font;
	sf::String mensaje;
	sf::Text chattingText;
	sf::Text text;
	sf::RectangleShape separator;
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

