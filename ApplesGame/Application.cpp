#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "GameSettings.h"
#include "Game.h"

namespace ApplesGame {
	sf::RenderWindow* Application::window;

	Application::Application() {
		window = new sf::RenderWindow(sf::VideoMode(ApplesGame::SCREEN_WIDTH, ApplesGame::SCREEN_HEGHT), "AppleGame");
	}

	Application::~Application() {
		delete window;
	}

	bool Application::isOpen() {
		return window->isOpen();
	}

	sf::RenderWindow& Application::GetWindow() { return *window; }

	void Application::HandleKeyReleasedEvent(sf::Event event)
	{
		auto state = State::Instance();

		switch (event.key.code)
		{
		case sf::Keyboard::Num1:
		case sf::Keyboard::Numpad1:
		{
			if (state->gameState.top() == GameState::MainMenu)
			{
				State::Instance()->ToggleGameMode((int)GameMode::infiniteApple);
			}
			else if (state->gameState.top() == GameState::PauseMenu)
			{
				state->gameState = {};
				state->gameState.push(GameState::MainMenu);
			}
			break;
		}
		case sf::Keyboard::Num2:
		case sf::Keyboard::Numpad2:
		{
			if (state->gameState.top() == GameState::MainMenu)
			{
				State::Instance()->ToggleGameMode((int)GameMode::withAcceleration);
			}
			else if (state->gameState.top() == GameState::PauseMenu)
			{
				window->close();
			}
			break;
		}
		case sf::Keyboard::Num3:
		{
			if (state->gameState.top() == GameState::PauseMenu)
			{
				state->gameState.pop();
			}
			break;
		}
		case sf::Keyboard::Space:
		{
			if (state->gameState.top() == GameState::PauseMenu)
			{
				state->gameState.pop();
			}
			else if (state->gameState.top() == GameState::GameOverMenu)
			{
				State::Instance()->Restart();
				state->gameState.pop();
			}
			else if (state->gameState.top() == GameState::MainMenu)
			{
				State::Instance()->Restart();
				state->gameState = {};
				state->gameState.push(GameState::Game);
			}
			else if (state->gameState.top() == GameState::Game)
			{
				state->gameState.push(GameState::PauseMenu);
			}
			break;
		}
		case sf::Keyboard::Tab:
		{
			if (state->gameState.top() == GameState::MainMenu) {
				state->gameState.push(GameState::Records);
			}
			else if (state->gameState.top() == GameState::Records) {
				state->gameState.pop();
			}
			break;
		}
		default:
			break;
		}
	}

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
				HandleKeyReleasedEvent(event);
			}
		}
	}

	int Application::Loop()
	{
		State::Instance()->Init();

		while (isOpen())
		{
			HandleWindowEvents();

			if (!isOpen())
			{
				return 0;
			}


			State::Instance()->player.HandleInput();

			// Calculate time delta
			sf::Time currentTime = game_clock.getElapsedTime();
			float timeDelta = currentTime.asSeconds() - lastTime.asSeconds();
			lastTime = currentTime;
			State::Instance()->Update(timeDelta);

			// Draw everything here
			// Clear the window first
			window->clear();

			State::Instance()->Draw(*window);

			// End the current frame, display window contents on screen
			window->display();
		}

		return 0;
	}
}