#pragma once
#include "SFML/Graphics.hpp"
#include <algorithm>
#include "MainMenu.h"
#include "Menu.h"
#include "Math.h"
#include "Game.h"
#include "Application.h"

using namespace sf;
using namespace std;

namespace ApplesGame
{
	MenuItem::MenuItem(string text, const float xOrigin)
	{
		textObj.setString(sf::String::fromUtf8(text.begin(), text.end()));
		textObj.setFont(State::Instance()->font);
		textObj.setCharacterSize(24);
		textObj.setFillColor(sf::Color::White);
		textObj.setOrigin(GetTextOrigin(textObj, { 0.5f, 0.f }));
	}

	void MenuItem::Hover()
	{
		textObj.setFillColor(sf::Color::Yellow);
	}

	void MenuItem::UnHover()
	{
		textObj.setFillColor(sf::Color::White);
	}
}