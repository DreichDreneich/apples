#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "Menu.h"
#include "Math.h"

namespace ApplesGame
{
	void Checkbox::InitCheckbox()
	{
		checkboxRect.setSize({ CheckboxSize, CheckboxSize });
		checkboxRect.setOutlineColor(sf::Color::White);
		checkboxRect.setFillColor(sf::Color::Black);
		checkboxRect.setOutlineThickness(2.f);
	}

	void Checkbox::InitMenuText(sf::String str, const sf::Font& font)
	{
		textObj.setFont(font);
		textObj.setCharacterSize(24);
		textObj.setFillColor(sf::Color::White);
		textObj.setString(str);
		textObj.setOrigin(GetTextOrigin(textObj, { 0.f, 0.5f }));
	}

	void Checkbox::Init(sf::String str, const sf::Font& font)
	{
		InitCheckbox();
		InitMenuText(str, font);
	}

	void Checkbox::Draw(sf::RenderWindow& window)
	{
		window.draw(textObj);
		window.draw(checkboxRect);
	}

	void TextField::Init(sf::String str, const sf::Font& font)
	{
		textObj.setFont(font);
		textObj.setCharacterSize(32);
		textObj.setFillColor(sf::Color::Yellow);
		textObj.setString(str);
		textObj.setOrigin(GetTextOrigin(textObj, { 0.5f, 0.5f }));
	}
}