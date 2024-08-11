#pragma once
#include <functional>
#include "MainMenu.h"
#include "Math.h"
#include "Checkbox.h"
#include "Game.h"
#include "UI.h"
#include "Application.h"
#include "Menu.h"

namespace ApplesGame
{
	void MenuPage::Init()
	{
		menu = new Menu();

		menu->AddItem("START", "Начать игру");
		menu->AddItem("DIFFICULTY", "Уровень сложности");
		menu->AddItem("RECORDS", "Рекорды");
		menu->AddItem("SETTINGS", "Настройки");
		menu->AddItem("EXIT", "Выход");

		menu->Hover(0);

		menu->OnSelect([](string& id) {
			if (id == "START") {
				State::Instance()->Restart();
				State::Instance()->gameState = {};
				State::Instance()->gameState.push(GameState::Game);
			}
			else if (id == "DIFFICULTY") {
				State::Instance()->gameState.push(GameState::DifficultyPage);
			}
			else if (id == "RECORDS") {
				State::Instance()->gameState.push(GameState::Records);
			}
			else if (id == "EXIT") {
				Application::Instance()->GetWindow().close();
			}
		});
	}

	void MenuPage::HandleKeyboardEvent(const sf::Event& evt)
	{
		menu->HandleKeyboardEvent(evt);
	}

	void MenuPage::Draw()
	{
		float windowX = (float)Application::Instance()->GetWindow().getSize().x / 2;
		float windowY = (float)Application::Instance()->GetWindow().getSize().y / 3;

		menu->Draw({ windowX, windowY });
	}
}