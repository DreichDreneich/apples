#pragma once
#include <functional>
#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "Math.h"
#include "Application.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	struct MenuItem {
		sf::Text textObj;

		MenuItem(string text, const float xOrigin = 1.f);

		void Hover();
		void UnHover();
	};

	template<typename T>
	class Menu {
	protected:
		vector<pair<T, MenuItem>> items;
		MenuItem* hoveredMenuItem;
		short hoveredMenuItemNumber;
		std::function<void(T&)> handleSelect;

		FloatRect GetMenuItemGlPositionById(const T& id) {
			auto it = find_if(items.begin(), items.end(), [id](pair<T, MenuItem> item) {
				return item.first == id;
				});

			return it->second.textObj.getGlobalBounds();
		}

	public:
		Menu() = default;
		
		void Hover(short number)
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

		void HandleKeyboardEvent(const sf::Event& evt) {
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
		void AddItem(T id, string text) {
			MenuItem item({ text });

			items.push_back({ id, item });
		}
		void OnSelect(std::function<void(T&)> func) {
			handleSelect = func;
		}
		void Draw(Vector2f pos) {
			for (int i = 0; i < items.size(); ++i) {
				items[i].second.textObj.setPosition({ pos.x, pos.y + 35 * i });
				Application::Instance()->GetWindow().draw(items[i].second.textObj);
			}
		}
	};

	template<typename T>
	class RadioMenu : public Menu<T>
	{
		T* selectedItem = nullptr;
		sf::RectangleShape selectedPointerRect;

	public:
		RadioMenu(T* item) {
			selectedPointerRect.setSize({ 10.f, 10.f });
			selectedPointerRect.setFillColor(sf::Color::Yellow);

			selectedItem = item;

			auto func = [&](T& id) {
				*selectedItem = id;
				};

			Menu<T>::OnSelect(func);
		}

		void Draw(Vector2f pos) {
			Menu<T>::Draw(pos);

			if (selectedItem != nullptr) {
				auto position = Menu<T>::GetMenuItemGlPositionById(*selectedItem);
				selectedPointerRect.setPosition({ position.left - 20.f, position.top + 4.f });
				Application::Instance()->GetWindow().draw(selectedPointerRect);
			}
		}

		void OnSelect(void(*func)(string id)) = delete;
	};

	struct CheckboxData {
		sf::RectangleShape rect;
		bool* isSelected;
	};

	template<typename T>
	class CheckboxMenu : public Menu<T>
	{
		unordered_map<T, CheckboxData> selectedItems;
		std::function<void(T&, bool)> callback;

	public:
		void Draw(Vector2f pos) {
			Menu<T>::Draw(pos);

			for (auto& item : selectedItems) {
				auto position = Menu<T>::GetMenuItemGlPositionById(item.first);
				item.second.rect.setPosition({ position.left - 20.f, position.top + 4.f });

				if (*item.second.isSelected)
				{
					Application::Instance()->GetWindow().draw(item.second.rect);
				}
			}
		}

		CheckboxMenu(void(*cb)(T id, bool currValue))
		{
			auto func = [&](T& id) {
				auto nextVal = !*selectedItems[id].isSelected;
				*selectedItems[id].isSelected = nextVal;
				callback(id, nextVal);
				};

			callback = cb;
			Menu<T>::OnSelect(func);
		};

		void AddItem(T id, string text, bool* isSelected) {
			Menu<T>::AddItem(id, text);

			sf::RectangleShape newRect;

			newRect.setSize({ 10.f, 10.f });
			newRect.setFillColor(sf::Color::Yellow);

			selectedItems.insert({ id, { newRect, isSelected } });
		}

		void OnSelect(void(*callback)(T id)) = delete;
	};
}