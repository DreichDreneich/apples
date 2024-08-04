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
	MenuItem::MenuItem(string text, const float xOrigin = 1.f)
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

	Menu::Menu()
	{

	}

	void Menu::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyReleased)
		{
			switch (evt.key.code) {
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
			{
				Hover(hoveredMenuItemNumber == 0 ? short(items.size() - 1) : hoveredMenuItemNumber - 1);
				break;
			}
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
			{
				auto a = hoveredMenuItemNumber == short(items.size() - 1) ? 0 : hoveredMenuItemNumber + 1;
				Hover(a);
				break;
			}
			case sf::Keyboard::Enter:
			{
				if (handleSelect != nullptr) {
					handleSelect(items[hoveredMenuItemNumber].first);
				}
				break;
			}
			}
		}
	}

	void Menu::AddItem(string id, string text)
	{
		MenuItem item({ text });

		items.push_back({ id, item });
	}

	void Menu::Hover(short number)
	{
		if (number > items.size() - 1) {
			return;
		}

		items[number].second.Hover();
		if (hoveredMenuItem != nullptr) {
			hoveredMenuItem->UnHover();
		}
		hoveredMenuItem = &items[number].second;
		hoveredMenuItemNumber = number;
	}

	void Menu::OnSelect(void(*func)(string id))
	{
		handleSelect = func;
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

		menu.Hover(0);

		auto func = [](string id) { 
			if (id == "START") {
				State::Instance()->Restart();
				State::Instance()->gameState = {};
				State::Instance()->gameState.push(GameState::Game);
			} 
			else if (id == "RECORDS") {
				State::Instance()->gameState.push(GameState::Records);
			}
			else if (id == "EXIT") {
				Application::Instance()->GetWindow().close();
			}
		};

		menu.OnSelect(func);
	}

	void MenuPage::HandleKeyboardEvent(const sf::Event& evt)
	{
		menu.HandleKeyboardEvent(evt);
	}

	void MenuPage::Draw(sf::RenderWindow& window)
	{
		float windowX = (float)window.getSize().x / 2;
		float windowY = (float)window.getSize().y / 3;

		menu.Draw({ windowX, windowY }, window);
	}
}