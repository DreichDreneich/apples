#pragma once
#include "SFML/Graphics.hpp"

namespace ApplesGame 
{
	class Page
	{
	public:
		// TODO: remove method
		virtual void Init() = 0;
		virtual void HandleKeyboardEvent(const sf::Event& event) = 0;
		virtual void Draw(Vector2f margin = {}) = 0;
	};
}