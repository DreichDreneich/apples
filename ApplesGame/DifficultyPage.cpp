#pragma once
#include <functional>
#include "DifficultyPage.h"
#include "Game.h"
#include "Application.h"
#include "Menu.h"
#include "UIComponents.h"

namespace ApplesGame
{
	void DifficultyPage::Init() 
	{
		header = new PageHeader("Уровень сложности");

		menu = new RadioMenu<Difficulty>(State::Instance()->getDifficulty());

		menu->AddItem(Difficulty::EASY, "Легкий");
		menu->AddItem(Difficulty::MEDIUM, "Средний");
		menu->AddItem(Difficulty::HARD, "Тяжелый");

		menu->Hover(0);

		backText = new CommonText("Нажмите <TAB> что бы вернуться назад", {0.5f, 0.f});
	}

	void DifficultyPage::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Tab)
		{
			State::Instance()->getGameState()->pop();
		}

		menu->HandleKeyboardEvent(evt);
	}

	void DifficultyPage::Draw(Vector2f margin)
	{
		float windowX = (float)Application::Instance()->GetWindow().getSize().x;
		float windowY = (float)Application::Instance()->GetWindow().getSize().y;

		menu->Draw({ windowX/2, windowY/3 });
		header->Draw();
		backText->Draw({ windowX / 2, windowY - 100 });
	}
}