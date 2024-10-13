#pragma once
#include "Player.h"
#include "BlocksGrid.h"
#include <unordered_map>

namespace ApplesGame {
	class GameStoreMemento {
	private:
		int score = 0;
		Platform _platform;
		Ball _ball;
		BlocksGrid _blocksGrid;
	public:
		GameStoreMemento(tuple<shared_ptr<Ball>, shared_ptr<Platform>, shared_ptr<BlocksGrid>, int> data) {
			_ball = *(get<0>(data).get());
			_platform = *(get<1>(data).get());
			_blocksGrid = *(get<2>(data).get());
			score = get<3>(data);

			_RPTF2(_CRT_WARN, "ball position x= %f, y= %f\n", _ball.GetPosition().x, _ball.GetPosition().y);
		}

		friend class GameStore;
		GameStoreMemento() {}
	};

	class GameStore {
		int score = 0;
		shared_ptr<Platform> _platform;
		shared_ptr<Ball> _ball;
		shared_ptr<BlocksGrid> _blocksGrid;

		unordered_map<string, shared_ptr<GameObject>>& gameObjects;
	public:
		void setGameScore(int sc) { score = sc; }
		int getGameScore() { return score; }

		shared_ptr<Platform> getPlatform() { return _platform; }
		Ball& getBall() { return *_ball; }
		BlocksGrid& getBlocksGrid() { return *_blocksGrid; }

		GameStore() = default;

		GameStore(unordered_map<string, shared_ptr<GameObject>>& _gameObjects) : gameObjects(_gameObjects) {
			_platform = make_shared<Platform>();
			_ball = make_shared<Ball>();

			gameObjects["platform"] = _platform;
			gameObjects["ball"] = _ball;
		}

		void Restart(const Texture& texture) {
			score = 0;

			createNewBlockGrid(texture);

			_platform->Move({ (SCREEN_WIDTH - 180.f) / 2.f, SCREEN_HEGHT - 50.f }); //TODO: remove magic numbers
			_platform->SetSpeed(450.f);

			_ball->Move({ (SCREEN_WIDTH - 10.f) / 2.f, SCREEN_HEGHT - 150.f }); //TODO: remove magic numbers
			_ball->SetSpeed(150.f);

			std::random_device rd; // Случайный генератор
			std::mt19937 gen(rd()); // Генератор псевдослучайных чисел Mersenne Twister
			std::uniform_real_distribution<float> dist(-0.9f, 0.9f);

			_ball->SetDirection({ dist(gen), -1.f });
		}

		tuple<bool, string> Update() {
			auto ballShape = _ball->GetShape();

			auto platformLines = GetRectLines(*_platform->GetShape(), _platform->GetPosition());
			auto line = findIntersectionCircleRectangle(_ball->GetPosition(), ballShape->getRadius(), platformLines);

			if (line != platformLines.end()) {
				auto nextDirection = reflectVector(_ball->GetPosition(), ballShape->getRadius(), _ball->GetDirection(), line->p1, line->p2);
				_ball->SetDirection(nextDirection);
			}

			auto grid = _blocksGrid->GetGrid();

			for (int i = 0; i < grid.size(); ++i) {
				for (int j = 0; j < grid[i].size(); ++j) {
					auto block = grid[i][j];

					if (block == nullptr) {
						continue;
					}

					auto blockLines = GetRectLines(*block->GetShape(), block->GetPosition());
					auto line = findIntersectionCircleRectangle(_ball->GetPosition(), ballShape->getRadius(), blockLines);

					if (line != blockLines.end()) {
						auto nextDirection = reflectVector(_ball->GetPosition(), ballShape->getRadius(), _ball->GetDirection(), line->p1, line->p2);
						_ball->SetDirection(nextDirection);

						block->ApplyDamage((short)1);
						if (block->GetHealth() == 0) {
							_blocksGrid->RemoveEl(i, j);
							++score;

							return { true, block->GetId() };
						}

						break;
					}
				}
			}

			return { false, "" };
		};

		GameStoreMemento* getGameSnapshot() {
			auto a = new GameStoreMemento({ _ball, _platform, _blocksGrid, score });
			return a;
		}

		void restoreGameSnapshot(GameStoreMemento* memento) {
			_RPTF2(_CRT_WARN, "ball position x= %f, y= %f\n", memento->_ball.GetPosition().x, memento->_ball.GetPosition().y);

			_ball = make_shared<Ball>(memento->_ball);
			auto oldBall = _ball->GetPosition();
			_ball->GetShape()->setRadius(memento->_ball.GetShape()->getRadius());

			gameObjects["ball"] = _ball;

			_platform = make_shared<Platform>(memento->_platform);
			_platform->GetShape()->setSize(memento->_platform.GetShape()->getSize());

			_blocksGrid = make_shared<BlocksGrid>(memento->_blocksGrid);
			addGridToGameObjects();

			score = memento->score;

			gameObjects["platform"] = _platform;
		}

		void addGridToGameObjects() {
			for (auto& blocksColumn : _blocksGrid->GetGrid()) {
				for (auto& block : blocksColumn) {
					gameObjects[block->GetId()] = block;
				}
			}
		}

		void createNewBlockGrid(const Texture& texture)
		{
			if (_blocksGrid != nullptr) {
				for (auto& blocksColumn : _blocksGrid->GetGrid()) {
					for (auto& block : blocksColumn) {
						gameObjects.erase(block->GetId());
					}
				}
			}

			_blocksGrid = make_unique<BlocksGrid>(texture);

			addGridToGameObjects();
		}
	};

	class GameCaretaker {
	private:
		GameStoreMemento _memento;
		bool hasSave = false;
	public:
		bool isSaveExists() { return hasSave; }

		GameStoreMemento* getMemento() {
			return &_memento;
		}
		void setMemento(GameStoreMemento& memento) {
			hasSave = true;
			_memento = memento;
		}
	};
}