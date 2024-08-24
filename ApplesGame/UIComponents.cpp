#pragma once
#include "UIComponents.h"
#include "Application.h"
#include "Game.h"

namespace ApplesGame
{
	void TextComponent::Draw(const Vector2f& position)
	{
		textObj.setPosition(position);
		Application::Instance()->GetWindow().draw(textObj);
	}

	PageHeader::PageHeader(string str, int size)
	{
		textObj.setFont(*State::Instance()->GetFont());
		textObj.setCharacterSize(size);
		textObj.setStyle(sf::Text::Bold);
		textObj.setFillColor(sf::Color::Yellow);
		textObj.setString(sf::String::fromUtf8(str.begin(), str.end()));
		textObj.setOrigin(GetTextOrigin(textObj, { 0.5f, 0.5f }));
	}

	CommonText::CommonText(const string& str, Vector2D origin)
	{
		textObj.setFont(*State::Instance()->GetFont());
		textObj.setCharacterSize(24);
		textObj.setFillColor(sf::Color::White);
		textObj.setString(sf::String::fromUtf8(str.begin(), str.end()));
		textObj.setOrigin(GetTextOrigin(textObj, origin));
	}

	void CommonText::setString(const string& str)
	{
		textObj.setString(sf::String::fromUtf8(str.begin(), str.end()));
		textObj.setOrigin(GetTextOrigin(textObj, { 0.5f, 0.5f }));
	}
}