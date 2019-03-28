#pragma once
#include "skeleton_gui.hpp"
#include <SFML/Graphics.hpp>

class GUITimeline
{
public:
	GUITimeline(GUISkeletonPtr skeleton):
		m_skeleton(skeleton)
	{}

	void draw(sf::RenderTarget* target)
	{

	}

private:
	GUISkeletonPtr m_skeleton;
};