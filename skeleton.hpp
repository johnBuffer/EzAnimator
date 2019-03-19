#pragma once
#include "vec2.h"
#include <vector>
#include <memory>

class Point
{
	Point(const up::Vec2& position) :
		m_position(position)
	{}

	Point(float x, float y):
		m_position(x, y)
	{}

	float m_radius;
	up::Vec2 m_position;
};

using PointPtr = std::shared_ptr<Point>;

class Connector
{
	Connector(const Point& position) :
		m_point(position),
		m_parent(nullptr)
	{}

	Connector(const Point& position, PointPtr parent) :
		m_point(position),
		m_parent(parent)
	{}

	Point m_point;
	PointPtr m_parent;

	std::vector<PointPtr> m_children;
};

using ConnectorPtr = std::shared_ptr<Connector>;

class Bone
{
	Bone(ConnectorPtr p1, ConnectorPtr p2):
		m_p1(p1),
		m_p2(p2)
	{}

	ConnectorPtr m_p1;
	ConnectorPtr m_p2;
};

using BonePtr = std::shared_ptr<Bone>;

