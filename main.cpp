#include <SFML/Graphics.hpp>
#include "GUI.hpp"

#include "skeleton.hpp"
#include "Header.h"

int main()
{
	constexpr uint32_t WIN_WIDTH = 1024;
	constexpr uint32_t WIN_HEIGHT = 768;
	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "EzAnimator");

	GUI gui(window);

	while (window.isOpen())
	{
		gui.handleEvents();

		window.clear();

		gui.draw();
		
		window.display();
	}

	return 0;
}