#pragma once
#include "SFML/Graphics.hpp"
#include "Checkbox.h"
#include "Menu.h"
#include "Page.h"

namespace ApplesGame
{
	enum class Pages {
		START, DIFFICULTY, RECORDS, EXIT, SETTINGS,
	};

	class MenuPage : Page {
		Menu<Pages>* menu;

	public:
		MenuPage() = default;

		void Init() override;
		void HandleKeyboardEvent(const sf::Event& event) override;
		void Draw(Vector2f margin = {}) override;
	};
}