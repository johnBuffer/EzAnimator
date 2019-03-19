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

class Bone
{
	Bone(PointPtr p1, PointPtr p2):
		m_p1(p1),
		m_p2(p2)
	{}

	PointPtr m_p1;
	PointPtr m_p2;
};

using BonePtr = std::shared_ptr<Bone>;

class Connector
{
	PointPtr m_parent;
	PointPtr m_point;

	std::vector<PointPtr> m_children;
	std::vector<BonePtr> m_bones;
};
