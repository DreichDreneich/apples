#include "RecordsListPage.h"
#include "Game.h"

namespace ApplesGame {
	void RecordsList::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Tab)
		{
			State::Instance()->getGameState()->pop();
		}
	}

	void RecordsList::Draw(Vector2f margin)
	{
		auto app = Application::Instance();
		auto gameState = State::Instance();

		vector<pair<string, int>> pairs(gameState->recordsList.begin(), gameState->recordsList.end());
		std::sort(pairs.begin(), pairs.end(), [](pair<string, int> a, pair<string, int> b) {
			return a.second > b.second;
			});

		for (short i = 0; i < pairs.size(); ++i)
		{
			auto recordTexts = recordsList[pairs[i].first];

			recordTexts.first->setString(pairs[i].first);
			recordTexts.second->setString(to_string(pairs[i].second));

			auto recordY = float(150 + i * 50);
			auto xTextCoord = app->GetWindow().getSize().x / 2.f + margin.x;

			recordTexts.first->Draw({ xTextCoord - 100.f, recordY + margin.y });
			recordTexts.second->Draw({ xTextCoord + 100.f, recordY + margin.y });
		}

		recordsListHeader->Draw({ app->GetWindow().getSize().x / 2.f + margin.x, 80.f + margin.y });

		float windowX = (float)app->GetWindow().getSize().x + margin.x;
		float windowY = (float)app->GetWindow().getSize().y + margin.y;

		backText->Draw({ windowX / 2, windowY - 100.f });
	}

	RecordsList::RecordsList()
	{
		recordsListData = &State::Instance()->recordsList;

		recordsListHeader = new PageHeader("Таблица рекордов");

		backText = new CommonText("<TAB> Назад", { 0.5f, 0.f });

		auto it = recordsListData->begin();

		while (it != recordsListData->end())
		{
			auto first = new CommonText({ it->first, { 0.f, 0.5f } });
			auto second = new CommonText({ to_string(it->second), { 0.f, 0.5f } });

			recordsList[it->first] = { first, second };
			++it;
		}
	}

	void RecordsList::Init() {

	}

	GameOverPage::GameOverPage() : RecordsList()
	{
		delete backText;
		delete recordsListHeader;

		recordsListHeader = new PageHeader("КОНЕЦ ИГРЫ");
		backText = new CommonText("Нажмите <Space> что бы начать игру заново\n	 и <TAB> что бы выйти в главное меню", { 0.5f, 0.f });
	}

	void GameOverPage::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyReleased)
		{
			if (evt.key.code == sf::Keyboard::Tab) 
			{
				State::Instance()->clearGameState();
				State::Instance()->getGameState()->push(GameState::MainMenu);
			}
			
			if (evt.key.code == sf::Keyboard::Space)
			{
				State::Instance()->getGameState()->pop();
				State::Instance()->Restart();
			}
		}
	}
}





