#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include "Math.h"
#include "UIComponents.h"
#include "Page.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	class RecordsList: public Page
	{
	protected:
		map<string, int>* recordsListData;

		PageHeader* recordsListHeader;
		map<string, pair<CommonText*, CommonText*>> recordsList;
		CommonText* backText;

	public:
		RecordsList();
		~RecordsList() {
			delete recordsListHeader;
			delete backText;
		};
		// Inherited via Page
		void Init() override;

		void HandleKeyboardEvent(const sf::Event& event) override;

		void Draw(Vector2f margin = {}) override;
	};

	class GameOverPage : public RecordsList
	{
	public:
		GameOverPage();
		void HandleKeyboardEvent(const sf::Event& event) override;
	};

	class WinPage : public GameOverPage {
	public:
		WinPage();
	};
}