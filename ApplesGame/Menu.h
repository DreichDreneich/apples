#pragma once
#include <functional>
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	struct MenuItem {
		sf::Text textObj;

		MenuItem(string text, const float xOrigin);

		void Hover();
		void UnHover();
	};

	class Menu {
	protected:
		vector<pair<string, MenuItem>> items;
		MenuItem* hoveredMenuItem;
		short hoveredMenuItemNumber;
		std::function<void(string&)> handleSelect;

		FloatRect* GetMenuItemGlPositionById(string& id);

	public:
		Menu();
		
		void Hover(short number);
		void HandleKeyboardEvent(const sf::Event& event);
		void AddItem(string id, string text);
		void OnSelect(std::function<void(string&)> func);
		void Draw(Vector2f pos);
	};

	class RadioMenu : public Menu
	{
		string* selectedItem = nullptr;
		sf::RectangleShape selectedPointerRect;

	public:
		RadioMenu(string* selectedItem = nullptr);

		void Draw(Vector2f pos);

		void OnSelect(void(*func)(string id)) = delete;
	};
}