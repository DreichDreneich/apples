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
		void Draw(const Vector2f& position = {});
	};

	class PageHeader: public TextComponent
	{
	public:
		PageHeader(string str, int size = 32);
	};

	class CommonText: public TextComponent {
	public:
		CommonText(const string& str, Vector2D origin = { 1.f, 0.f });
		void setString(const string& str);
	};
}
