#pragma once

#include "Game.h"
#include "GameField.h"
#include "Application.h"

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

	void GameField::Draw(State& gameState)
	{
		auto gs = State::Instance();

		sf::RectangleShape border(sf::Vector2f(SCREEN_WIDTH - 4, SCREEN_HEGHT - TOP_PADDING - 4));
		border.setPosition(2, TOP_PADDING + 2);
		border.setOutlineThickness(2);
		border.setOutlineColor(sf::Color(70, 148, 0));
		border.setFillColor(sf::Color::Black);
		Application::Instance()->GetWindow().draw(border);

		sf::RectangleShape shape(sf::Vector2f(SCREEN_WIDTH, TOP_PADDING));
		shape.setPosition(0, 0);
		shape.setFillColor(sf::Color(70, 148, 0, 220));
		Application::Instance()->GetWindow().draw(shape);


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
				actor.Draw();
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

		//Application::Instance()->GetWindow().draw(b1, 3, sf::Lines);
		//Application::Instance()->GetWindow().draw(b2, 3, sf::Lines);
		//Application::Instance()->GetWindow().draw(b3, 3, sf::Lines);
		//Application::Instance()->GetWindow().draw(b4, 3, sf::Lines);
	}
}