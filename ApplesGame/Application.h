#pragma once
#include <SFML/Graphics.hpp>

namespace ApplesGame {
	class Application {
		static sf::RenderWindow* window;
		sf::Clock game_clock;
		sf::Time lastTime = game_clock.getElapsedTime();
		float timeDelta = 0.f;

		Application();
		static Application* _instance;
		//GameCaretaker gameCaretaker;
	public:

		Application(Application& other) = delete;
		Application operator=(const Application&) = delete;

		static Application* Instance();

		~Application();

		sf::RenderWindow& GetWindow();

		bool isOpen();

		void HandleWindowEvents();

		int Loop();
	};
}