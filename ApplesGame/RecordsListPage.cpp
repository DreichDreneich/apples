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

			recordTexts.first->Draw({
				app->GetWindow().getSize().x / 2.f - 100 + margin.x,
				recordY + margin.y 
				});

			recordTexts.second->Draw({
				app->GetWindow().getSize().x / 2.f + 100 + margin.x,
				recordY + margin.y
				});
		}

		recordsListHeader->Draw({ app->GetWindow().getSize().x / 2.f + margin.x, 100.f + margin.y });

		float windowX = (float)app->GetWindow().getSize().x + margin.x;
		float windowY = (float)app->GetWindow().getSize().y + margin.y;

		backText->Draw({ windowX / 2, windowY - 100 });
	}

	RecordsList::RecordsList()
	{
		recordsListData = &State::Instance()->recordsList;

		recordsListHeader = new PageHeader("Таблица рекордов");

		backText = new CommonText("Нажмите <TAB> что бы выйти", { 0.5f, 0.f });
	}

	void RecordsList::Init() {
		auto it = recordsListData->begin();

		while (it != recordsListData->end())
		{
			auto first = new CommonText({ it->first, { 0.f, 0.5f } });
			auto second = new CommonText({ to_string(it->second), { 0.f, 0.5f } });

			recordsList[it->first] = { first, second };
			++it;
		}
	}

	GameOverPage::GameOverPage()
	{
		delete backText;
		delete recordsListHeader;

		recordsListHeader = new PageHeader("КОНЕЦ ИГРЫ");
		backText = new CommonText("Нажмите <Space> что бы начать игру заново и <TAB> что бы выйти в главное меню", { 0.5f, 0.f });
	}
}





