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
		menu = new Menu<Pages>();

		menu->AddItem(Pages::START, "Начать игру");
		menu->AddItem(Pages::DIFFICULTY, "Уровень сложности");
		menu->AddItem(Pages::RECORDS, "Рекорды");
		menu->AddItem(Pages::SETTINGS, "Настройки");
		menu->AddItem(Pages::EXIT, "Выход");

		menu->Hover(0);

		menu->OnSelect([](Pages& id) {
			if (id == Pages::START) {
				State::Instance()->Restart();
				State::Instance()->gameState = {};
				State::Instance()->gameState.push(GameState::Game);
			}
			else if (id == Pages::DIFFICULTY) {
				State::Instance()->gameState.push(GameState::DifficultyPage);
			}
			else if (id == Pages::RECORDS) {
				State::Instance()->gameState.push(GameState::Records);
			}
			else if (id == Pages::EXIT) {
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