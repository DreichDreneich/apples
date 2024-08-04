#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>

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
		vector<pair<string, MenuItem>> items;
		MenuItem* hoveredMenuItem;
		short hoveredMenuItemNumber;
		void(*handleSelect)(string id);

	public:
		Menu();
		
		void Hover(short number);
		void HandleKeyboardEvent(const sf::Event& event);
		void AddItem(string id, string text);
		void OnSelect(void(*func)(string id));
		void Draw(Vector2f pos, sf::RenderWindow& window);
	};

	class MenuPage{
		Menu menu;

	public:
		MenuPage() = default;

		void Init();
		void HandleKeyboardEvent(const sf::Event& event);
		void Draw(sf::RenderWindow& window);
	};
}