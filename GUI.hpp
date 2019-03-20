#pragma once
#include <SFML/Graphics.hpp>
#include "skeleton.hpp"

class GUI
{
public:
	GUI(sf::RenderWindow& window);

	void handleEvents();

private:
	sf::RenderWindow& m_window;
};

class GUIBone
{
public:
	GUIBone(BonePtr bone) :
		m_bone(bone)
	{}

private:
	BonePtr m_bone;
};

class GUIConnector
{
public:
	GUIConnector(ConnectorPtr connector) :
		m_connector(connector)
	{}

	void draw(sf::RenderTarget* target)
	{
		constexpr float radius = 15.0f;
		const up::Vec2& pos(m_connector->position());
		sf::CircleShape c(radius);
		c.setOrigin(radius, radius);
		c.setPosition(pos.x, pos.y);

		target->draw(c);
	}

private:
	ConnectorPtr m_connector;
};
