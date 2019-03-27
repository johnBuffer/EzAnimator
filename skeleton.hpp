#pragma once
#include "vec2.h"
#include <vector>
#include <memory>
#include <iostream>

constexpr float DEFAULT_BONE_LENGTH = 75.0f;

class Connector;
class Bone;
class Skeleton;

using ConnectorPtr = std::shared_ptr<Connector>;
using BonePtr = std::shared_ptr<Bone>;
using SkeletonPtr = std::shared_ptr<Skeleton>;
using PointPtr = std::shared_ptr<up::Vec2>;

// A connector is point to which it is possible to connect a bone
class Connector
{
public:
	Connector(const up::Vec2& position) :
		m_point(position),
		m_parent(nullptr),
		m_current_key(0),
		m_keys({ 0.0f })
	{}

	Connector(const up::Vec2& position, ConnectorPtr parent) :
		m_point(position),
		m_parent(parent),
		m_current_key(0)
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
		m_angle = angle - m_parent->angle();
		std::cout << m_angle << std::endl;
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

	void update(float dt)
	{
		if (m_parent && !m_keys.empty())
		{
			float current_angle = m_keys[m_current_key];
			float next_angle = nextAngle();

			float delta = next_angle - current_angle;
			//if (stddelta)

			setRelAngle(current_angle + delta * dt);
		}
	}

	float nextAngle()
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
	}

private:
	// Geometry
	up::Vec2 m_point;
	float m_angle;

	uint32_t m_current_key;
	std::vector<float> m_keys;

	void updatePosition()
	{
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

// A bone consist of a distance constraint between two connectors
class Bone
{
public:
	Bone(ConnectorPtr p1, ConnectorPtr p2):
		m_p1(p1),
		m_p2(p2)
	{}

private:
	ConnectorPtr m_p1;
	ConnectorPtr m_p2;
};

// A skeleton is a hierachy of bones and connectors
class Skeleton
{
public:
	Skeleton(const up::Vec2& position = up::Vec2()) :
		m_root(std::make_shared<Connector>(position))
	{}

	void addBone(ConnectorPtr attach)
	{
		up::Vec2 new_connector_position(attach->point() + up::Vec2(0, DEFAULT_BONE_LENGTH));

		// Create new bone and connector
		std::shared_ptr<Connector> new_connector = std::make_shared<Connector>(new_connector_position, attach);
		std::shared_ptr<Bone>      new_bone      = std::make_shared<Bone>(attach, new_connector);

		// Add them to the skeleton
		m_connectors.push_back(new_connector);
		m_bones.push_back(new_bone);
	}

	ConnectorPtr addConnector(ConnectorPtr parent, const up::Vec2& position)
	{
		ConnectorPtr new_connector(std::make_shared<Connector>(position, parent));
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
	std::vector<BonePtr> m_bones;
};

