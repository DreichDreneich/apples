﻿// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)
#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>

#include "Game.h"

using namespace ApplesGame;

void HandleKeyReleasedEvent(sf::Event event, sf::RenderWindow& window)
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
			window.close();
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
		else if(state->gameState.top() == GameState::GameOverMenu)
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

void HandleWindowEvents(sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		// Close window if close button or Escape key pressed
		if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
		{
			window.close();
		}

		if (event.type == sf::Event::KeyReleased)
		{
			HandleKeyReleasedEvent(event, window);
		}
	}
}

int main()
{
	// Init random number generator
	unsigned int seed = (unsigned int)time(nullptr); // Get current time as seed. You can also use any other number to fix randomization
	srand(seed);

	// Init window
	sf::RenderWindow window(sf::VideoMode(ApplesGame::SCREEN_WIDTH, ApplesGame::SCREEN_HEGHT), "AppleGame");


	auto gameState = State::Instance();

	// Init game clock
	sf::Clock game_clock;
	sf::Time lastTime = game_clock.getElapsedTime();

	// Game loop
	while (window.isOpen())
	{
		HandleWindowEvents(window);

		if (!window.isOpen())
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
		window.clear();

		State::Instance()->Draw(window);

		// End the current frame, display window contents on screen
		window.display();
	}

	return 0;
}
