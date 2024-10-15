#pragma once
#include <vector>
#include "Player.h"
#include "GameSettings.h"
#include <random>
#include <iostream>

using namespace std;

namespace ApplesGame {
	class BlocksGrid {
	private:
		vector<vector<shared_ptr<Block>>> grid;

		float blocksMargin = 10.f;

		float blockWidth = 0.f;
		float blockHeight = NUM_Y <= 4 ? 50.f : 200 / NUM_Y;
	public:
		// TODO: move to settings


		BlocksGrid() = default;

		BlocksGrid(const Texture& texture) {
			grid.resize(NUM_X);

			for (auto& blocksColumn : grid) {
				blocksColumn.resize(NUM_Y);
			}

			Fill(texture);
		}

		BlocksGrid(BlocksGrid& g) {
			grid.resize(g.grid.size());

			for (int i = 0; i < g.grid.size(); ++i) {
				for (int j = 0; j < g.grid[i].size(); ++j) {
					if (j == 0) {
						grid[i].resize(g.grid[i].size());
					}
					if (g.grid[i][j] != nullptr) {
						// grid[i][j] = make_shared<Block>(*g.grid[i][j].get()); // Does not work
						grid[i][j] = g.grid[i][j]->clone();
						grid[i][j]->GetShape()->setSize(g.grid[i][j]->GetShape()->getSize());
					}
				}
			}
		}

		BlocksGrid& operator=(const BlocksGrid& g) {
			grid.resize(g.grid.size());

			for (int i = 0; i < g.grid.size(); ++i) {
				for (int j = 0; j < g.grid[i].size(); ++j) {
					if (j == 0) {
						grid[i].resize(g.grid[i].size());
					}
					if (g.grid[i][j] != nullptr) {
						// grid[i][j] = make_shared<Block>(*g.grid[i][j].get()); // Does not work
						grid[i][j] = g.grid[i][j]->clone();
						grid[i][j]->GetShape()->setSize(g.grid[i][j]->GetShape()->getSize());
					}
				}
			}

			return *this;
		}

		~BlocksGrid() {
			for (auto blocksColumn : grid) {
				blocksColumn.clear();
			}

			grid.clear();
		}

		vector<vector<shared_ptr<Block>>>& GetGrid() {
			return grid;	
		}

		void RemoveEl(int i, int j) {
			auto& column = grid[i];
			column.erase(column.begin() + j);
		}

		void Fill(const Texture& texture) {
			blockWidth = (SCREEN_WIDTH - ((float)(NUM_X + 1) * blocksMargin)) / (float)NUM_X;


			std::random_device rd; // Случайный генератор
			std::mt19937 gen(rd()); // Генератор псевдослучайных чисел Mersenne Twister
			std::uniform_int_distribution<short> dist(1, 10);

			for (int i = 0; i < grid.size(); ++i) {
				for (int j = 0; j < grid[i].size(); ++j) {
					auto distGen = dist(gen);
					const bool isStrongBlock = distGen > 5;
					//const bool isStrongBlock = dist(gen) > 2;
					grid[i][j] = isStrongBlock ? make_shared<StrongBlock>(texture) : make_shared<Block>();
					grid[i][j]->GetShape()->setSize({ blockWidth, blockHeight });
					grid[i][j]->Move({ blocksMargin + i * blocksMargin + i * blockWidth, blocksMargin + j * blocksMargin + j * blockHeight });
				}
			}
		}
	};
}