#pragma once
#include "UIComponents.h"
#include "Application.h"
#include "Game.h"

namespace ApplesGame
{
	void TextComponent::Draw()
	{
		Application::Instance()->GetWindow().draw(textObj);
	}

	PageHeader::PageHeader(string str)
	{
		textObj.setFont(State::Instance()->font);
		textObj.setCharacterSize(32);
		textObj.setStyle(sf::Text::Bold);
		textObj.setFillColor(sf::Color::Yellow);
		textObj.setString(sf::String::fromUtf8(str.begin(), str.end()));
		textObj.setOrigin(GetTextOrigin(textObj, { 0.5f, 0.5f }));

		textObj.setPosition(Application::Instance()->GetWindow().getSize().x / 2.f, 100.f);
	}

	CommonText::CommonText(string str, Vector2D origin)
	{
		textObj.setFont(State::Instance()->font);
		textObj.setCharacterSize(24);
		textObj.setFillColor(sf::Color::White);
		textObj.setString(sf::String::fromUtf8(str.begin(), str.end()));
		textObj.setOrigin(GetTextOrigin(textObj, origin));
	}

	void CommonText::Draw(float x, float y)
	{
		textObj.setPosition(x, y);
		TextComponent::Draw();
	}
}