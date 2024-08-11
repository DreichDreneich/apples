#pragma once
#include "SFML/Graphics.hpp"
#include "Math.h"

using namespace sf;
using namespace std;

namespace ApplesGame
{
	class TextComponent
	{
	protected:
		Text textObj;

	public:
		void Draw();
	};

	class PageHeader: public TextComponent
	{
	public:
		PageHeader(string str);
	};

	class CommonText: public TextComponent {
	public:
		CommonText(string str, Vector2D origin = { 1.f, 0.f });

		void Draw(float x, float y);
		void Draw() = delete;
	};
}
