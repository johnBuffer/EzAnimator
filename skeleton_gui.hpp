#pragma once

#include "skeleton.hpp"
#include <SFML/Graphics.hpp>

class GUIConnector;
class GUISkeleton;

using GUIConnectorPtr = std::shared_ptr<GUIConnector>;
using GUISkeletonPtr = std::shared_ptr<GUISkeleton>;

class GUIConnector
{
public:
	GUIConnector(ConnectorPtr connector) :
		m_connector(connector),
		m_selected(false)
	{}

	void draw(sf::RenderTarget* target)
	{
		constexpr float radius = 8.0f;
		constexpr float selection_radius = 16.0f;

		const up::Vec2& pos(m_connector->point());

		if (m_connector->parent())
		{
			const up::Vec2& pos_parent(m_connector->parent()->point());
			sf::VertexArray va(sf::Lines, 2);
			va[0].position = sf::Vector2f(pos.x, pos.y);
			va[1].position = sf::Vector2f(pos_parent.x, pos_parent.y);

			target->draw(va);
		}
		
		if (m_selected)
		{
			sf::CircleShape c_selection(selection_radius);
			c_selection.setFillColor(sf::Color::Red);
			c_selection.setOrigin(selection_radius, selection_radius);
			c_selection.setPosition(pos.x, pos.y);

			target->draw(c_selection);
		}

		sf::CircleShape c(radius);
		c.setFillColor(sf::Color::Green);
		c.setOrigin(radius, radius);
		c.setPosition(pos.x, pos.y);

		target->draw(c);
	}

	void setAngle(float angle)
	{
		m_connector->setAngle(angle);
	}

	ConnectorPtr connector()
	{
		return m_connector;
	}

	void selected(bool is_selected)
	{
		m_selected = is_selected;
	}

	void update(float dt)
	{
		m_connector->update(dt);
	}

	void nextKey()
	{
		m_connector->nextKey();
	}

private:
	ConnectorPtr m_connector;
	bool m_selected;
};


class GUISkeleton
{
public:
	GUISkeleton(const up::Vec2& position = up::Vec2()) :
		m_skeleton(position),
		m_root_connector(std::make_shared<GUIConnector>(m_skeleton.root()))
	{
		m_connectors.push_back(m_root_connector);
	}
	
	~GUISkeleton() = default;

	void draw(sf::RenderTarget* target)
	{
		for (GUIConnectorPtr connector : m_connectors)
		{
			connector->draw(target);
		}
	}

	void update(float time)
	{
		for (GUIConnectorPtr connector : m_connectors)
		{
			connector->update(time);
		}
	}

	void nextKey()
	{
		for (GUIConnectorPtr connector : m_connectors)
		{
			connector->nextKey();
		}
	}

	void makeKey()
	{
		for (GUIConnectorPtr connector : m_connectors)
		{
			connector->connector()->makeKey();
		}
	}

	void reset()
	{
		for (GUIConnectorPtr connector : m_connectors)
		{
			connector->connector()->reset();
		}
	}

	GUIConnectorPtr getConnectorAt(const up::Vec2& position)
	{
		for (GUIConnectorPtr connector : m_connectors)
		{
			const up::Vec2& point(connector->connector()->point());
			const up::Vec2& v(position - point);

			if (v.length() < 8)
			{
				return connector;
			}
		}

		return nullptr;
	}

	void addConnector(GUIConnectorPtr parent)
	{
		ConnectorPtr new_connector(m_skeleton.addConnector(parent->connector()));
		m_connectors.emplace_back(std::make_shared<GUIConnector>(new_connector));
	}

	GUIConnectorPtr root()
	{
		return m_root_connector;
	}


private:
	Skeleton m_skeleton;
	GUIConnectorPtr m_root_connector;
	std::vector<GUIConnectorPtr> m_connectors;
};
