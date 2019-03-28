#pragma once
#include "vec2.h"
#include <vector>
#include <memory>
#include <iostream>

constexpr float DEFAULT_BONE_LENGTH = 75.0f;

class Connector;
class Skeleton;

using ConnectorPtr = std::shared_ptr<Connector>;
using SkeletonPtr = std::shared_ptr<Skeleton>;
using PointPtr = std::shared_ptr<up::Vec2>;

constexpr float PI = 3.141592653f;

// A connector is point to which it is possible to connect a bone
class Connector
{
public:
	Connector(const up::Vec2& position) :
		m_parent(nullptr),
		m_point(position),
		m_current_key(0),
		m_angle(0.0f)
	{}

	Connector(ConnectorPtr parent) :
		m_parent(parent),
		m_current_key(0),
		m_angle(0.0f)
	{
		if (m_parent)
		{
			updatePosition();
		}
	}

	const up::Vec2& point() const
	{
		return m_point;
	}

	ConnectorPtr parent()
	{
		return m_parent;
	}

	float angle() const
	{
		if (m_parent)
		{
			return m_angle + m_parent->angle();
		}
		
		return 0.0f;
	}

	void setAngle(float angle)
	{
		m_angle = fmod(angle - m_parent->angle(), 2*PI);
		updatePosition();
	}

	void setRelAngle(float angle)
	{
		m_angle = angle;
		updatePosition();
	}

	void addChild(ConnectorPtr child)
	{
		m_children.emplace_back(child);
	}

	void addKey(float angle)
	{
		m_keys.push_back(angle);
	}

	void update(float ratio)
	{
		if (m_parent && !m_keys.empty())
		{
			// Fetch current angle
			float current_angle = m_keys[m_current_key];
			
			// Get next angle
			float next_angle = getNextAngle();

			// Compute delta between the two 
			float delta = next_angle - current_angle;

			// Check if delta is the shortest one
			if (fabs(delta) > PI)
			{
				if (current_angle < 0)
					current_angle += 2 * PI;
				else
					next_angle += 2 * PI;
				
				// If not update
				delta = next_angle - current_angle;
			}

			// Interpolate angles
			setRelAngle(current_angle + delta * ratio);
		}
	}

	float getNextAngle()
	{
		if (m_current_key < m_keys.size() - 1)
		{
			return m_keys[m_current_key + 1];
		}

		return m_keys[0];
	}

	void nextKey()
	{
		(++m_current_key) %= m_keys.size();
	}

	void makeKey()
	{
		m_keys.push_back(m_angle);
	}

	void reset()
	{
		m_current_key = m_keys.size() - 1;
		m_angle = m_keys[m_current_key];
		updatePosition();
	}

private:
	// Geometry
	up::Vec2 m_point;
	float m_angle;

	uint32_t m_current_key;
	std::vector<float> m_keys;

	void updatePosition()
	{
		if (!m_parent)
			return;

		// Get abs angle
		float base_angle = this->angle();

		// Update position accordingly
		up::Vec2 new_rel_position(DEFAULT_BONE_LENGTH*cos(base_angle), DEFAULT_BONE_LENGTH*sin(base_angle));
		m_point = m_parent->point() + new_rel_position;

		// Update children
		for (ConnectorPtr child : m_children)
		{
			child->updatePosition();
		}
	}

	// Architecture
	ConnectorPtr m_parent;
	std::vector<ConnectorPtr> m_children;
};

// A skeleton is a hierachy of bones and connectors
class Skeleton
{
public:
	Skeleton(const up::Vec2& position = up::Vec2()) :
		m_root(std::make_shared<Connector>(position))
	{}

	ConnectorPtr addConnector(ConnectorPtr parent)
	{
		ConnectorPtr new_connector(std::make_shared<Connector>(parent));
		parent->addChild(new_connector);
		m_connectors.push_back(new_connector);

		return new_connector;
	}

	ConnectorPtr root()
	{
		return m_root;
	}

private:
	ConnectorPtr m_root;
	std::vector<ConnectorPtr> m_connectors;
};

