#pragma once
#include "SFML/Graphics.hpp"
#include <algorithm>
#include "MainMenu.h"
#include "Menu.h"
#include "Math.h"
#include "Game.h"

using namespace sf;
using namespace std;

namespace ApplesGame
{
	MenuItem::MenuItem(string text, const sf::Font& font, const float xOrigin = 1.f)
	{
		textObj.setString(sf::String::fromUtf8(text.begin(), text.end()));
		textObj.setFont(font);
		textObj.setCharacterSize(24);
		textObj.setFillColor(sf::Color::White);
		textObj.setOrigin(GetTextOrigin(textObj, { 0.5f, 0.f }));
	}

	Menu::Menu()
	{

	}

	void Menu::Update()
	{

	}

	void Menu::AddItem(string id, string text, const sf::Font& font)
	{
		MenuItem item({ text, font });
		items.emplace(id, item);
	}

	void Menu::Hover(string id)
	{
		auto nextHovered = items.find(id);

		if (nextHovered == items.end()) {
			return;
		}

		(*nextHovered).second.textObj.setFillColor(sf::Color::Yellow);
		if (hoveredMenuItem != nullptr) {
			hoveredMenuItem->textObj.setFillColor(sf::Color::White);
		}
		hoveredMenuItem = &(*nextHovered).second;
	}

	void Menu::OnSelect(void(*func)(string id))
	{
		func(hoveredMenuItemId);
	}

	void Menu::Draw(Vector2f pos, sf::RenderWindow& window)
	{
		auto it = items.begin();
		auto i = 0;

		while (it != items.end()) {
			(*it).second.textObj.setPosition({ pos.x, pos.y + 35 * i });
			window.draw((*it).second.textObj);

			++i;
			++it;
		}
	}

	MenuPage::MenuPage(const sf::Font& font)
	{
		menu.AddItem("START", "Start игру", font);
		menu.AddItem("DIFFICULTY", "Уровень сложности", font);
		menu.AddItem("RECORDS", "Таблица рекордов", font);
		menu.AddItem("SETTINGS", "Настройки", font);
		menu.AddItem("EXIT", "Выход", font);

		menu.Hover("START");

		auto func = [](string id) { 
			if (id == "START") {
				State::Instance()->gameState.push(GameState::Game);
			}

		};

		menu.OnSelect(func);
	}

	void MenuPage::Update()
	{

	}

	void MenuPage::Draw(sf::RenderWindow& window)
	{
		float windowX = (float)window.getSize().x / 2;
		float windowY = (float)window.getSize().y / 3;

		menu.Draw({ windowX, windowY }, window);
	}
}