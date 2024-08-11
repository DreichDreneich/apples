#pragma once
#include <functional>
#include "DifficultyPage.h"
#include "Game.h"
#include "Application.h"
#include "Menu.h"
#include "UIComponents.h"
#include "GameSettings.h"

namespace ApplesGame
{
	void DifficultyPage::handleSelect(string id) {
		if (id == "START") {
			State::Instance()->Restart();
			State::Instance()->gameState = {};
			State::Instance()->gameState.push(GameState::Game);
		}
		else if (id == "RECORDS") {
			State::Instance()->gameState.push(GameState::Records);
		}
		else if (id == "EXIT") {
			Application::Instance()->GetWindow().close();
		}
	};

	void DifficultyPage::Init() 
	{
		header = new PageHeader("Уровень сложности");

		menu = new RadioMenu(State::Instance()->getDifficulty());

		menu->AddItem(DIFFICULTY_EASY, "Легкий");
		menu->AddItem(DIFFICULTY_MEDIUM, "Средний");
		menu->AddItem(DIFFICULTY_HARD, "Тяжелый");

		menu->Hover(0);

		backText = new CommonText("Нажмите <TAB> что бы вернуться назад", {0.5f, 0.f});
	}

	void DifficultyPage::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Tab)
		{
			State::Instance()->gameState.pop();
		}

		menu->HandleKeyboardEvent(evt);
	}

	void DifficultyPage::Draw()
	{
		float windowX = (float)Application::Instance()->GetWindow().getSize().x;
		float windowY = (float)Application::Instance()->GetWindow().getSize().y;

		menu->Draw({ windowX/2, windowY/3 });
		header->Draw();
		backText->Draw(windowX / 2, windowY - 100);
	}
}