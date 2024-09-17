#pragma once
#include <vector>
#include "Player.h"
#include "GameSettings.h"

using namespace std;

namespace ApplesGame {
	class BlocksGrid {
	private:
		vector<vector<shared_ptr<Block>>> grid;

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
					block.reset();
				}
				blocksColumn.clear();
			}

			grid.clear();
		}

		vector<vector<shared_ptr<Block>>>& GetGrid() {
			return grid;	
		}

		void RemoveEl(int i, int j) {
			auto& column = grid[i];
			auto& el = column[j];
			el.reset();
			column.erase(column.begin() + j);
		}

		void Fill(const Texture& texture) {
			blockWidth = (SCREEN_WIDTH - ((float)(NUM_X + 1) * blocksMargin)) / (float)NUM_X;

			for (int i = 0; i < grid.size(); ++i) {
				for (int j = 0; j < grid[i].size(); ++j) {
					grid[i][j] = make_shared<Block>(Block(texture));
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