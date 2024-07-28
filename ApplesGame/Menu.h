#pragma once
#include "SFML/Graphics.hpp"

namespace ApplesGame
{
	const float CheckboxSize = 10.f;

	class Checkbox {
	public:
		sf::Text textObj;
		bool isChecked;
		sf::RectangleShape checkboxRect;

		void Init(sf::String str, const sf::Font& font);
		void Draw(sf::RenderWindow& window);

	private:
		void InitCheckbox();
		void InitMenuText(sf::String str, const sf::Font& font);
	};

	class TextField {
	public:
		sf::Text textObj;
		void Init(sf::String str, const sf::Font& font);
	};
}