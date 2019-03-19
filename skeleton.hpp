#pragma once
#include "vec2.h"
#include <vector>
#include <memory>

// Base class representing a 2D point
class Point
{
public:
	Point(const up::Vec2& position) :
		m_position(position)
	{}

	Point(float x, float y):
		m_position(x, y)
	{}

private:
	// Radius is used for physics and GUI purpose
	float m_radius;
	up::Vec2 m_position;
};
using PointPtr = std::shared_ptr<Point>;

// A connector is point to which it is possible to connect a bone
class Connector
{
public:
	Connector(const Point& position) :
		m_point(position),
		m_parent(nullptr)
	{}

	Connector(const Point& position, PointPtr parent) :
		m_point(position),
		m_parent(parent)
	{}

public:
	Point m_point;
	PointPtr m_parent;

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

	void addBone(ConnectorPtr attach);

private:
	Connector m_root;
	std::vector<ConnectorPtr> m_connectors;
	std::vector<BonePtr> m_bones;
};

