#pragma once
#include <SFML/Graphics.hpp>
#include "skeleton_gui.hpp"
#include "event_manager.hpp"

class GUI
{
public:
	GUI(sf::RenderWindow& window) :
		m_skeleton(up::Vec2(512, 300)),
		m_window(window),
		m_event_manager(window),
		m_root(nullptr),
		m_selected(nullptr),
		m_clicked(false),
		m_playing(false)
	{
		m_event_manager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event&) {m_window.close(); });
		m_event_manager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event&) { clic(); });
		m_event_manager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event&) { unclic(); });
		m_event_manager.addKeyReleasedCallback(sf::Keyboard::A, [&](const sf::Event&) { addConnector(); });
		m_event_manager.addKeyReleasedCallback(sf::Keyboard::K, [&](const sf::Event&) { m_skeleton.makeKey(); });
		m_event_manager.addKeyReleasedCallback(sf::Keyboard::Space, [&](const sf::Event&) { m_playing = !m_playing; if (!m_playing) { m_skeleton.reset(); } });

		m_root = m_skeleton.root();
		select(m_root);
	}

	void handleEvents()
	{
		m_mouse_position = sf::Mouse::getPosition(m_window);
		m_event_manager.processEvents();

		if (m_clicked)
		{
			if (m_selected && m_selected->connector()->parent())
			{
				const up::Vec2 mouse(m_mouse_position.x, m_mouse_position.y);
				const up::Vec2 v(mouse - m_selected->connector()->parent()->point());

				m_selected->setAngle(v.angle());
			}
		}

		if (m_playing)
		{
			m_time += 0.016f;
			if (m_time >= 1.0f)
			{
				m_time = 0.0f;
				m_skeleton.nextKey();
			}

			m_skeleton.update(m_time);
		}
	}

	void addConnector()
	{
		if (m_selected)
		{
			m_skeleton.addConnector(m_selected);
		}
	}

	GUIConnectorPtr getConnectorAtMouse()
	{
		const up::Vec2 position(m_mouse_position.x, m_mouse_position.y);

		return m_skeleton.getConnectorAt(position);
	}

	void draw()
	{
		m_skeleton.draw(&m_window);
	}

	void select(GUIConnectorPtr connector)
	{
		if (m_selected)
			m_selected->selected(false);

		if (connector)
			connector->selected(true);
		
		m_selected = connector;
	}

private:
	sfev::EventManager m_event_manager;
	sf::RenderWindow& m_window;

	GUIConnectorPtr m_root;
	GUIConnectorPtr m_selected;

	GUISkeleton m_skeleton;

	bool m_clicked;
	sf::Vector2i m_mouse_position;
	sf::Vector2i m_clic_position;

	float m_time;
	bool m_playing;

	void clic()
	{
		m_clicked = true;
		m_clic_position = m_mouse_position;

		select(getConnectorAtMouse());
	}

	void unclic()
	{
		m_clicked = false;
	}
};
