#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "Menu.h"
#include "Math.h"

namespace ApplesGame
{
	void InitCheckbox(sf::RectangleShape& checkbox)
	{
		checkbox.setSize({ CheckboxSize, CheckboxSize });
		checkbox.setOutlineColor(sf::Color::White);
		checkbox.setFillColor(sf::Color::Black);
		checkbox.setOutlineThickness(2.f);
	}

	void InitMenuText(sf::Text& menuText, sf::String str, const sf::Font& font)
	{
		menuText.setFont(font);
		menuText.setCharacterSize(24);
		menuText.setFillColor(sf::Color::White);
		menuText.setString(str);
		menuText.setOrigin(GetTextOrigin(menuText, { 0.f, 0.5f }));
	}

	void InitMenuItem(MenuItem& menuItem, sf::String str, const sf::Font& font)
	{
		InitCheckbox(menuItem.checkbox);
		InitMenuText(menuItem.textObj, str, font);
	}
}