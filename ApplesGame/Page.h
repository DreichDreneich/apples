#pragma once
#include "SFML/Graphics.hpp"

namespace ApplesGame 
{
	class Page
	{
	public:
		virtual void Init() = 0;
		virtual void HandleKeyboardEvent(const sf::Event& event) = 0;
		virtual void Draw() = 0;
	};
}