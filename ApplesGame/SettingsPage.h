#pragma once

#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "Page.h"
#include "UIComponents.h"

namespace ApplesGame
{
	enum class SettingsFlags {
		isSoundOn, isMusicOn,
	};

	class SettingsPage : Page {
		PageHeader* header;
		CheckboxMenu<SettingsFlags>* menu;
		CommonText* backText;

	public:
		SettingsPage();
		~SettingsPage() {
			delete header;
			delete menu;
			delete backText;
		};

		void Init() override;
		void HandleKeyboardEvent(const sf::Event& event) override;
		void Draw(Vector2f margin = {}) override;
	};
}