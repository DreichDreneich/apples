#pragma once

#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "Page.h"
#include "UIComponents.h"

namespace ApplesGame
{
	class DifficultyPage : Page {
		PageHeader* header;
		RadioMenu* menu;
		CommonText* backText;


		void handleSelect(string id);

	public:
		DifficultyPage() = default;

		void Init();
		void HandleKeyboardEvent(const sf::Event& event);
		void Draw();
	};
}