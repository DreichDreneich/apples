#pragma once
#include <SFML/Graphics.hpp>

namespace ApplesGame {
	class Application {
		static sf::RenderWindow* window;
		sf::Clock game_clock;
		sf::Time lastTime = game_clock.getElapsedTime();

	public:

		Application();

		~Application();

		static sf::RenderWindow& GetWindow();

		bool isOpen();

		void HandleKeyReleasedEvent(sf::Event event);

		void HandleWindowEvents();

		int Loop();
	};
}