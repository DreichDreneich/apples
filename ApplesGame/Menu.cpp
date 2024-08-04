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
	MenuItem::MenuItem(string text, const float xOrigin = 1.f)
	{
		textObj.setString(sf::String::fromUtf8(text.begin(), text.end()));
		textObj.setFont(State::Instance()->font);
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

	void Menu::AddItem(string id, string text)
	{
		MenuItem item({ text });
		items.push_back({ id, item });
	}

	void Menu::Hover(string id)
	{
		auto nextHovered = find_if(items.begin(), items.end(), [id](pair<string, MenuItem> el) {
			return el.first == id;
		});

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
		for(int i = 0; i < items.size(); ++i) {
			items[i].second.textObj.setPosition({ pos.x, pos.y + 35 * i });
			window.draw(items[i].second.textObj);
		}
	}

	void MenuPage::Init()
	{
		menu.AddItem("START", "Start игру");
		menu.AddItem("DIFFICULTY", "Уровень сложности");
		menu.AddItem("RECORDS", "Таблица рекордов");
		menu.AddItem("SETTINGS", "Настройки");
		menu.AddItem("EXIT", "Выход");

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