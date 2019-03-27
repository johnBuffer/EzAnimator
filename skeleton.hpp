#pragma once
#include "vec2.h"
#include <vector>
#include <memory>

constexpr float DEFAULT_BONE_LENGTH = 150.0f;

// Base class representing a 2D point
using PointPtr = std::shared_ptr<up::Vec2>;

// A connector is point to which it is possible to connect a bone
class Connector
{
public:
	Connector(const up::Vec2& position) :
		m_point(std::make_shared<up::Vec2>(position)),
		m_parent(nullptr),
		m_angle(0.0f)
	{}

	Connector(const up::Vec2& position, PointPtr parent) :
		m_point(std::make_shared<up::Vec2>(position)),
		m_parent(parent),
		m_angle(0.0f)
	{}

	const up::Vec2& position() const
	{
		return *m_point;
	}

	PointPtr point()
	{
		return m_point;
	}

	PointPtr parent()
	{
		return m_parent;
	}

public:
	PointPtr m_point;
	PointPtr m_parent;

	float m_angle;

	std::vector<PointPtr> m_children;
};
using ConnectorPtr = std::shared_ptr<Connector>;

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
using BonePtr = std::shared_ptr<Bone>;

// A skeleton is a hierachy of bones and connectors
class Skeleton
{
public:
	Skeleton() :
		m_root({ 0.0f, 0.0f })
	{}

	void addBone(ConnectorPtr attach)
	{
		PointPtr parent_position(attach->point());
		up::Vec2 new_connector_position(*parent_position + up::Vec2(0, DEFAULT_BONE_LENGTH));

		// Create new bone and connector
		std::shared_ptr<Connector> new_connector = std::make_shared<Connector>(new_connector_position, parent_position);
		std::shared_ptr<Bone>      new_bone      = std::make_shared<Bone>(attach, new_connector);

		// Add them to the skeleton
		m_connectors.push_back(new_connector);
		m_bones.push_back(new_bone);
	}

private:
	Connector m_root;
	std::vector<ConnectorPtr> m_connectors;
	std::vector<BonePtr> m_bones;
};

