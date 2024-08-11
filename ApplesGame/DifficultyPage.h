#pragma once

#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "Page.h"
#include "UIComponents.h"

namespace ApplesGame
{
	enum class Difficulty {
		EASY, MEDIUM, HARD,
	};

	class DifficultyPage : Page {
		PageHeader* header;
		RadioMenu<Difficulty>* menu;
		CommonText* backText;

	public:
		DifficultyPage() = default;

		void Init();
		void HandleKeyboardEvent(const sf::Event& event);
		void Draw();
	};
}