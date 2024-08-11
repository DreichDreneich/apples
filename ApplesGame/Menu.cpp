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

	FloatRect* Menu::GetMenuItemGlPositionById(string& id)
	{
		for (auto item : items) {
			if (item.first == id) {
				auto a = item.second.textObj.getGlobalBounds();
				return &a;
			}
		}

		return nullptr;
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

	void Menu::OnSelect(std::function<void(string&)> func)
	{
		handleSelect = func;
	}

	void Menu::Draw(Vector2f pos)
	{
		for(int i = 0; i < items.size(); ++i) {
			items[i].second.textObj.setPosition({ pos.x, pos.y + 35 * i });
			Application::Instance()->GetWindow().draw(items[i].second.textObj);
		}
	}

	RadioMenu::RadioMenu(string* selectedItemRef)
	{
		selectedPointerRect.setSize({ 10.f, 10.f });
		selectedPointerRect.setFillColor(sf::Color::Yellow);

		RadioMenu::selectedItem = selectedItemRef;

		auto func = [&](string& id) {
			RadioMenu::selectedItem = &id;
		};

		Menu::OnSelect(func);

	}

	void RadioMenu::Draw(Vector2f pos)
	{
		Menu::Draw(pos);

		if (selectedItem != nullptr) {
			auto position = GetMenuItemGlPositionById(*selectedItem);
			if (position != nullptr) {
				selectedPointerRect.setPosition({ position->left - 20.f, position->top + 4.f });
				Application::Instance()->GetWindow().draw(selectedPointerRect);
			}
		}
	}
}