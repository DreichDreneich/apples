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
	};

	class Menu {
		vector<pair<string, MenuItem>> items;
		MenuItem* hoveredMenuItem;
		string hoveredMenuItemId;

	public:
		Menu();
		
		void Update();
		void AddItem(string id, string text);
		void Hover(string id);
		void OnSelect(void(*func)(string id));
		void Draw(Vector2f pos, sf::RenderWindow& window);
	};

	class MenuPage{
		Menu menu;

	public:
		MenuPage() = default;

		void Init();
		void Update();
		void Draw(sf::RenderWindow& window);
	};
}