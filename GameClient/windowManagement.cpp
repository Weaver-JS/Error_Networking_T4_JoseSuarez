#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	


	FreeConsole();
	
	serverOrClient.initClient();
	
	

	screenDimensions.x = x;
	screenDimensions.y = y;
	windowRenderer.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), chatName);

	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}
	mensaje = " >";

	chattingText.setString(mensaje);
	chattingText.setFont(font);
	chattingText.setCharacterSize(14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);

	enemyChattingText.setString(mensaje);
	enemyChattingText.setFont(font);
	enemyChattingText.setCharacterSize(14);
	enemyChattingText.setFillColor(sf::Color(0, 160, 0));
	enemyChattingText.setStyle(sf::Text::Bold);

	timer.setString(mensaje);
	timer.setFont(font);
	timer.setCharacterSize(14);
	timer.setFillColor(sf::Color(0, 160, 0));
	timer.setStyle(sf::Text::Bold);
	timer.setPosition(0, (puntuationSeparator.getPosition().y + 25));

	
	word.setString("BACALAO");
	word.setFont(font);
	word.setCharacterSize(14);
	word.setFillColor(sf::Color(0, 160, 0));
	word.setStyle(sf::Text::Bold);
	word.setPosition(0, (puntuationSeparator.getPosition().y + 50));

	text.setString(mensaje);
	text.setFont(font);
	text.setCharacterSize(14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, screenDimensions.y - (screenDimensions.y / 4) + 50);

	textName.setString(serverOrClient.getName());
	textName.setFont(font);
	textName.setCharacterSize(14);
	textName.setFillColor(sf::Color(0, 160, 0));
	textName.setStyle(sf::Text::Bold);
	textName.setPosition(0, (nameUpseparator.getPosition().y + 150));

	enemyName.setString(serverOrClient.getName());
	enemyName.setFont(font);
	enemyName.setCharacterSize(14);
	enemyName.setFillColor(sf::Color(0, 160, 0));
	enemyName.setStyle(sf::Text::Bold);
	enemyName.setPosition(400, (nameUpseparator.getPosition().y + 150));

	separator.setSize(sf::Vector2f(screenDimensions.x, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, screenDimensions.y - screenDimensions.y/4);

	separatorup.setSize(sf::Vector2f(5, screenDimensions.y- screenDimensions.y/8));
	separatorup.setFillColor(sf::Color(200, 200, 200, 255));
	separatorup.setPosition(screenDimensions.x / 2, screenDimensions.y / 8);
	
	nameUpseparator.setSize(sf::Vector2f(screenDimensions.x, 5));
	nameUpseparator.setFillColor(sf::Color(200, 200, 200, 255));
	nameUpseparator.setPosition(0, screenDimensions.y / 8);
	
	namedownSeparator.setSize(sf::Vector2f(screenDimensions.x, 5));
	namedownSeparator.setFillColor(sf::Color(200, 200, 200, 255));
	namedownSeparator.setPosition(0, nameUpseparator.getPosition().y + 100);
	
	puntuationSeparator.setSize(sf::Vector2f(5, nameUpseparator.getPosition().y - screenDimensions.y));
	puntuationSeparator.setFillColor(sf::Color(200, 200, 200, 255));
	puntuationSeparator.setPosition(screenDimensions.x - screenDimensions.x / 4, nameUpseparator.getPosition().y);
}

void windowManagement::loop()
{

	sf::Thread recv_thread(&Client::recv, &serverOrClient);
	recv_thread.launch();
	while (serverOrClient.isConnected())
	{
		switch (serverOrClient.getState())
		{
		case	gameStates::CONNECT:
			break;
		default:
			while (windowRenderer.isOpen())
			{

				sf::Event evento;
				while (windowRenderer.pollEvent(evento))
				{
					switch (evento.type)
					{
					case sf::Event::Closed:
						windowRenderer.close();
						break;
					case sf::Event::KeyPressed:
						if (evento.key.code == sf::Keyboard::Escape)
							windowRenderer.close();
						else if (evento.key.code == sf::Keyboard::Return)
						{

							serverOrClient.send(mensaje);

							if (aMensajes.size() > 25)
							{
								aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
							}
							if (enemyMessages.size() > 25)
							{
								enemyMessages.erase(enemyMessages.begin(), enemyMessages.begin() + 1);
							}
							mensaje = ">";
						}
						break;
					case sf::Event::TextEntered:
						if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
							mensaje += (char)evento.text.unicode;
						else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
						{
							mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
							std::string mensaje_ = mensaje + "_";
							//	serverOrClient.sendMessage(mensaje_);
						}
						break;
					}
				}
			

				if (!serverOrClient.isConnected())
				{
					windowRenderer.close();
					break;
				}
				if (serverOrClient.getCleanBool())
				{
					aMensajes.clear();
					enemyMessages.clear();

					serverOrClient.setCleanBool(false);
				}

				if (serverOrClient.getMessageRecieved().length() > 0)
				{
					if (serverOrClient.getName() != "" && serverOrClient.getMymessage().length() > 0)
					{
						aMensajes.push_back(serverOrClient.getName() + " " +serverOrClient.getMymessage());
						if (aMensajes.size() > 25)
						{
							aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
						}
						serverOrClient.getMymessage().clear();
					}

					if (serverOrClient.getEnemyName() != "" && serverOrClient.getEnemyMessage().length() > 0)
					{
						enemyMessages.push_back(serverOrClient.getEnemyName() + " " + serverOrClient.getEnemyMessage());
						if (enemyMessages.size() > 25)
						{
							enemyMessages.erase(enemyMessages.begin(), enemyMessages.begin() + 1);
						}
						serverOrClient.getEnemyMessage().clear();


					}
				}

				if (serverOrClient.getPlayer().getWin())
				{
					aMensajes.clear();
					enemyMessages.clear();
					aMensajes.push_back(serverOrClient.getName() + " WINS");
					enemyMessages.push_back(serverOrClient.getName() + " WINS");
				}
				if (serverOrClient.getPlayer().getLose())
				{
					aMensajes.clear();
					enemyMessages.clear();
					aMensajes.push_back(serverOrClient.getEnemyName() + "WINS");
					enemyMessages.push_back(serverOrClient.getEnemyName() + "WINS");
				}

				windowRenderer.draw(separator);
				windowRenderer.draw(separatorup);
				windowRenderer.draw(nameUpseparator);
				windowRenderer.draw(namedownSeparator);
				windowRenderer.draw(puntuationSeparator);

				for (size_t i = 0; i < aMensajes.size(); i++)
				{

					std::string chatting = aMensajes[i];
					chattingText.setPosition(sf::Vector2f(0, 20 * i + namedownSeparator.getPosition().y + 25));
					chattingText.setString(chatting);
					windowRenderer.draw(chattingText);

				}
			
			
			for (size_t i = 0; i < enemyMessages.size(); i++)
			{

				std::string chatting = enemyMessages[i];
				enemyChattingText.setPosition(sf::Vector2f(400, 20 * i + namedownSeparator.getPosition().y + 25));
				enemyChattingText.setString(chatting);
				windowRenderer.draw(enemyChattingText);

			}

			std::string mensaje_ = mensaje + "_";
			text.setString(mensaje_);
			

			
			textName.setString(serverOrClient.getName()+ ": " + std::to_string(serverOrClient.getPlayer().getPuntuation() ));
			enemyName.setString(serverOrClient.getEnemyName() + ": " + std::to_string(serverOrClient.getPlayer().getEnemyPuntuation()));
			timer.setString(std::to_string(serverOrClient.getTime()));
			word.setString(serverOrClient.getPlayer().getactualWord());
			windowRenderer.draw(textName);
			windowRenderer.draw(enemyName);
			windowRenderer.draw(text);
			windowRenderer.draw(timer);
			windowRenderer.draw(word);
			//NETWORKING



		


				//NETWORKING
				windowRenderer.display();
				windowRenderer.clear();
			}
			//	recv_thread.terminate();
			recv_thread.terminate();
			if (serverOrClient.isConnected())
				serverOrClient.closeConnections();

			break;

		}
		
	}
}




sf::RenderWindow & windowManagement::getWindowRenderer()
{
	
	return get().windowRenderer;
}


windowManagement::~windowManagement()
{
}
