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
			auto state = State::Instance();

			if (id == Pages::START) {
				state->Restart();
				state->clearGameState();
				state->getGameState()->push(GameState::Game);
			}
			else if (id == Pages::DIFFICULTY) {
				state->getGameState()->push(GameState::DifficultyPage);
			}
			else if (id == Pages::RECORDS) {
				state->getGameState()->push(GameState::Records);
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

	void MenuPage::Draw(Vector2f margin)
	{
		float windowX = (float)Application::Instance()->GetWindow().getSize().x / 2;
		float windowY = (float)Application::Instance()->GetWindow().getSize().y / 3;

		menu->Draw({ windowX, windowY });
	}
}