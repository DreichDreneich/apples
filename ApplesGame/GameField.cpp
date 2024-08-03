#pragma once

#include "Game.h"
#include "GameField.h"

#include <assert.h>

using namespace std;

namespace ApplesGame
{
	Vector2f GameField::FieldToScreenPosition(Vector2i pos)
	{
		return {
			float(pos.x * FIELD_CELL_SIZE + ACTOR_SIZE * ORIGIN_MULTIPLIER),
			float(TOP_PADDING + pos.y * FIELD_CELL_SIZE + ACTOR_SIZE * ORIGIN_MULTIPLIER)
		};
	}

	void GameField::Draw(State& gameState, sf::RenderWindow& window)
	{
		auto gs = State::Instance();

		for (int i = 0; i < gs->gameField.grid.size(); i++)
		{
			for (int j = 0; j < gs->gameField.grid[j].size(); j++)
			{
				auto el = gs->gameField.grid[i][j];

				if (el.type == ActorType::NONE) {
					continue;
				}

				auto actor = gs->actorsInfo[el.type].store.at(el.idx);

				actor.position = GameField::FieldToScreenPosition({ i, j });
				actor.Draw(window);
			}
		}

		sf::Vertex b1[] =
		{
			sf::Vertex(sf::Vector2f(0, TOP_PADDING)),
			sf::Vertex(sf::Vector2f(SCREEN_WIDTH, TOP_PADDING)),
		};

		sf::Vertex b2[] =
		{
			sf::Vertex(sf::Vector2f(1, TOP_PADDING)),
			sf::Vertex(sf::Vector2f(1, SCREEN_HEGHT)),
		};

		sf::Vertex b3[] =
		{
			sf::Vertex(sf::Vector2f(1, SCREEN_HEGHT - 1)),
			sf::Vertex(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEGHT - 1)),
		};

		sf::Vertex b4[] =
		{
			sf::Vertex(sf::Vector2f(SCREEN_WIDTH, TOP_PADDING)),
			sf::Vertex(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEGHT)),
		};

		window.draw(b1, 3, sf::Lines);
		window.draw(b2, 3, sf::Lines);
		window.draw(b3, 3, sf::Lines);
		window.draw(b4, 3, sf::Lines);
	}
}