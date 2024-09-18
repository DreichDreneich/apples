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
		vector<vector<Block*>> grid;

	public:
		// TODO: move to settings
		float blocksMargin = 10.f;

		float blockWidth = 0.f;
		float blockHeight = NUM_Y <= 4 ? 50.f : 200 / NUM_Y;

		BlocksGrid() = default;

		BlocksGrid(const Texture& texture) {
			grid.resize(NUM_X);

			for (auto& blocksColumn : grid) {
				blocksColumn.resize(NUM_Y);
			}

			Fill(texture);
		}

		~BlocksGrid() {
			for (auto blocksColumn : grid) {
				for (auto block : blocksColumn) {
					delete block;
				}
				blocksColumn.clear();
			}

			grid.clear();
		}

		vector<vector<Block*>>& GetGrid() {
			return grid;	
		}

		void RemoveEl(int i, int j) {
			auto& column = grid[i];
			auto& el = column[j];
			delete el;
			column.erase(column.begin() + j);
		}

		void Fill(const Texture& texture) {
			blockWidth = (SCREEN_WIDTH - ((float)(NUM_X + 1) * blocksMargin)) / (float)NUM_X;


			std::random_device rd; // Случайный генератор
			std::mt19937 gen(rd()); // Генератор псевдослучайных чисел Mersenne Twister
			std::uniform_int_distribution<short> dist(1, 10);

			for (int i = 0; i < grid.size(); ++i) {
				for (int j = 0; j < grid[i].size(); ++j) {
					bool isStrongBlock = dist(gen) > 8;
					grid[i][j] = isStrongBlock ? new StrongBlock(texture) : new Block();
					grid[i][j]->GetShape()->setSize({ blockWidth, blockHeight });
					grid[i][j]->Move({ blocksMargin + i * blocksMargin + i * blockWidth, blocksMargin + j * blocksMargin + j * blockHeight });
				}
			}
		}

		void Draw() {
			for (auto& blocksColumn : grid) {
				for (auto block : blocksColumn) {
					block->Draw();
				}
			}
		}
	};
}