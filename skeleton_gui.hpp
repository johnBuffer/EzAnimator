#pragma once

#include "skeleton.hpp"
#include <SFML/Graphics.hpp>

class GUIBone;
class GUIConnector;
class GUISkeleton;

using GUIConnectorPtr = std::shared_ptr<GUIConnector>;
using GUIBonePtr = std::shared_ptr<GUIBone>;
using GUISkeletonPtr = std::shared_ptr<GUISkeleton>;

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
		m_connector(connector),
		m_selected(false)
	{}

	void draw(sf::RenderTarget* target)
	{
		constexpr float radius = 8.0f;
		constexpr float selection_radius = radius * 1.4f;
		
		const up::Vec2& pos(m_connector->point());

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

	void addConnector(GUIConnectorPtr parent, const up::Vec2& position)
	{
		ConnectorPtr new_connector(m_skeleton.addConnector(parent->connector(), position));
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
