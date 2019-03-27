#pragma once
#include <SFML/Graphics.hpp>
#include "skeleton_gui.hpp"

class GUI
{
public:
	GUI(sf::RenderWindow& window);

	void handleEvents();

	void addConnector(const up::Vec2& position)
	{

	}

private:
	sf::RenderWindow& m_window;
};
