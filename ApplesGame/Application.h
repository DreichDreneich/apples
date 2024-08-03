#pragma once
#include <SFML/Graphics.hpp>

namespace ApplesGame {
	class Application {
		sf::RenderWindow* window;
		sf::Clock game_clock;
		sf::Time lastTime = game_clock.getElapsedTime();

	public:

		Application();

		~Application();

		sf::RenderWindow& GetWindow();

		bool isOpen();

		void HandleKeyReleasedEvent(sf::Event event);

		void HandleWindowEvents();

		int Loop();
	};
}