#include "GUI.hpp"

GUI::GUI(sf::RenderWindow& target) :
	m_window(target)
{
}

void GUI::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}
