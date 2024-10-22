#pragma once

#include <string>
#include "UI.h"
#include "MainMenu.h"
#include "Game.h"
#include "Menu.h"
#include "RecordsListPage.h"
#include "SettingsPage.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	void InitGameTitle(Text& gameTitle)
	{
		gameTitle.setFont(State::Instance()->GetFont());
		gameTitle.setCharacterSize(48);
		gameTitle.setFillColor(sf::Color::Red);
		gameTitle.setStyle(sf::Text::Bold);
		string str = "Змейка";
		gameTitle.setString(String::fromUtf8(str.begin(), str.end()));
		gameTitle.setOrigin(GetTextOrigin(gameTitle, { 0.5f, 0.5f }));
	}

	// TODO: use TextComponent
	void InitRegularText(sf::Text& text, const string str)
	{
		text.setFont(State::Instance()->GetFont());
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setOrigin(GetTextOrigin(text, { 0.f, 0.f }));
		text.setString(sf::String::fromUtf8(str.begin(), str.end()));
	}

	void InitPauseGame(PauseGameMenu& pauseGameMenu)
	{
		InitRegularText(pauseGameMenu.mainMenu, "<1> Главное меню");
		InitRegularText(pauseGameMenu.quitGame, "<2> Выйти из игры");
		InitRegularText(pauseGameMenu.resumeGame, "<3> Продолжить игру");
	}

	void UIState::DrawHint()
	{
		inputHintText.setPosition(window->getSize().x - 10.f, 10.f);
		window->draw(inputHintText);
	}

	void UIState::DrawPauseMenu()
	{
		//TODO: find longest string
		auto mainMenuItemWidth = pauseGameMenu.resumeGame.getLocalBounds().width / 2;

		float windowX = (float)window->getSize().x / 2;
		float windowY = (float)window->getSize().y / 3;

		gameTitle.setPosition(windowX, 60.f);

		auto xWithOffset = windowX - mainMenuItemWidth;

		pauseGameMenu.mainMenu.setPosition(xWithOffset, windowY);
		pauseGameMenu.quitGame.setPosition(xWithOffset, windowY + 35.f);
		pauseGameMenu.resumeGame.setPosition(xWithOffset, windowY + 70.f);

		window->draw(pauseGameMenu.mainMenu);
		window->draw(pauseGameMenu.quitGame);
		window->draw(pauseGameMenu.resumeGame);
		window->draw(gameTitle);
	}

	UIState::UIState()
	{
		difficultyPage = new DifficultyPage();
		border = sf::RectangleShape(sf::Vector2f(SCREEN_WIDTH - 4, SCREEN_HEGHT - TOP_PADDING - 4));
		border.setPosition(2, TOP_PADDING + 2);
		border.setOutlineThickness(2);
		border.setOutlineColor(sf::Color(70, 148, 0));
		border.setFillColor(sf::Color::Black);
	}

	void UIState::DrawGameBorder() {
		
		Application::Instance()->GetWindow().draw(border);

		sf::RectangleShape shape(sf::Vector2f(SCREEN_WIDTH, TOP_PADDING));
		shape.setPosition(0, 0);
		shape.setFillColor(sf::Color(70, 148, 0, 220));
		Application::Instance()->GetWindow().draw(shape);
	}

	void UIState::InitUI(sf::RenderWindow* window)
	{
		UIState::window = window;

		InitGameTitle(gameTitle);

		scoreText.setFont(State::Instance()->GetFont());
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Black);
		scoreText.setStyle(sf::Text::Bold);

		inputHintText.setFont(State::Instance()->GetFont());
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::Black);
		string hintStr = "Используйте стрелки что бы двигаться,\nSpace для паузы, ESC для выхода, <F5> для сохранения";
		inputHintText.setString(String::fromUtf8(hintStr.begin(), hintStr.end()));
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

		isBonusDurationVisible = false;
		bonusDuration.setFont(State::Instance()->GetFont());
		bonusDuration.setCharacterSize(14);
		bonusDuration.setFillColor(sf::Color::White);

		menuPage.Init();
		difficultyPage->Init();
		gameOverPage = make_unique<GameOverPage>();
		recordsList = make_unique<RecordsList>();
		settingsPage = make_unique<SettingsPage>();
		winPage = make_unique<WinPage>();

		InitPauseGame(pauseGameMenu);
	}

	void UIState::Update()
	{
		string scoreStr = "Счет: ";
		scoreText.setString(String::fromUtf8(scoreStr.begin(), scoreStr.end()) + std::to_string(State::Instance()->GetScore()));

		sf::Color gameOverTextColor = (int)State::Instance()->timeSinceGameOver % 2 ? sf::Color::Red : sf::Color::Yellow;

		auto appliedBonuses = State::Instance()->getGameStore()->GetAppliedBonuses();

		BonusType removedBonusType;
		for (auto& textInfo : bonusesTexts) {
			if (appliedBonuses.find(textInfo.first) == appliedBonuses.end()) {
				removedBonusType = textInfo.first;
				break;
			}
		}

		bonusesTexts.erase(removedBonusType);

		for (auto& bonus : appliedBonuses) {
			string title;

			if (bonus.first == BonusType::GLASS_BLOCKS) {
				title = "Glass blocks: ";
			}
			else if (bonus.first == BonusType::FIREBALL) {
				title = "Fireball: ";
			}
			else if (bonus.first == BonusType::FAST_PLATFORM) {
				title = "Fast platform: ";
			}
			else {
				title = "Bonus: ";
			}

			if (bonusesTexts.find(bonus.first) == bonusesTexts.end()) {
				auto text = make_shared<SmallText>(title + std::to_string((int)bonus.second->durationRemained));
				bonusesTexts.insert({ bonus.first, text });
			}
			else {
				bonusesTexts[bonus.first]->setString(title + std::to_string((int)bonus.second->durationRemained));
			}
		}
	}

	void UIState::Draw()
	{
		switch (State::Instance()->getGameState().top())
		{
		case GameState::MainMenu:
		{
			menuPage.Draw();

			gameTitle.setPosition((float)window->getSize().x / 2, 100.f);
			window->draw(gameTitle);

			break;
		}
		case GameState::DifficultyPage:
		{
			difficultyPage->Draw();
			break;
		}
		case GameState::WinPage:
		{
			winPage->Draw();
			break;
		}
		case GameState::GameOverMenu:
		{
			gameOverPage->Draw();
			break;
		}
		case GameState::SettingsPage:
		{
			settingsPage->Draw();
			break;
		}
		case GameState::Game:
		{
			DrawGameBorder();

			if (isBonusDurationVisible) 
			{
				bonusDuration.setPosition(bonusDurationPosition);
				window->draw(bonusDuration);
			}

			scoreText.setPosition(10.f, 10.f);
			window->draw(scoreText);

			int count = 0;
			for (auto& bonus : bonusesTexts) {
				bonus.second->Draw({ 50.f, SCREEN_HEGHT - 100.f - (count * 30.f) });
				++count;
			}

			inputHintText.setPosition(window->getSize().x - 10.f, 3.f);
			window->draw(inputHintText);

			break;
		}
		case GameState::Records:
		{
			recordsList->Draw();
			break;
		}
		case GameState::PauseMenu:
		{
			DrawPauseMenu();
			break;
		}
		}
	}
}

