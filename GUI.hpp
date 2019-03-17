#pragma once
#include <SFML/Graphics.hpp>

class GUI
{
public:
	GUI(sf::RenderWindow& window);

	void handleEvents();

private:
	sf::RenderWindow& m_window;
};
