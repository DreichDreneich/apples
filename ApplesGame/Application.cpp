#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "GameSettings.h"
#include "Game.h"

namespace ApplesGame {
	Application* Application::_instance;

	sf::RenderWindow* Application::window;

	Application::Application() {
		window = new sf::RenderWindow(sf::VideoMode(ApplesGame::SCREEN_WIDTH, ApplesGame::SCREEN_HEGHT), "AppleGame");
	}

	Application* Application::Instance()
	{
		if (_instance == nullptr) {
			_instance = new Application();
			return _instance;
		}

		return _instance;
	}

	Application::~Application() {
		delete window;
	}

	bool Application::isOpen() {
		return window->isOpen();
	}

	sf::RenderWindow& Application::GetWindow() { return *window; }

	void Application::HandleWindowEvents()
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			// Close window if close button or Escape key pressed
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
				window->close();
			}

			if (event.type == sf::Event::KeyReleased)
			{
				State::Instance()->HandleKeyReleasedEvent(event);
			}

			State::Instance()->HandleKeyboardEvent(event);
		}
	}

	int Application::Loop()
	{
		State::Instance()->Init(*window);

		while (isOpen())
		{
			HandleWindowEvents();

			if (!isOpen())
			{
				return 0;
			}


			State::Instance()->getPlayer()->HandleInput();

			// Calculate time delta
			sf::Time currentTime = game_clock.getElapsedTime();
			float timeDelta = currentTime.asSeconds() - lastTime.asSeconds();
			lastTime = currentTime;
			State::Instance()->Update(timeDelta);

			// Draw everything here
			// Clear the window first
			window->clear();

			State::Instance()->Draw();

			// End the current frame, display window contents on screen
			window->display();
		}

		return 0;
	}
}