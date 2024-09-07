#pragma once
#include <vector>
#include "Player.h"

using namespace std;

namespace ApplesGame {
	class BlocksGrid {
	private:
		vector<vector<Block*>> grid;

	public:
		// TODO: move to settings
		int Numx = 8;
		int Numy = 3;
		float blocksMargin = 10.f;

		float blockWidth = 0.f;
		float blockHeight = 50.f;

		BlocksGrid() {
			grid.resize(Numx);

			for (auto& blocksColumn : grid) {
				blocksColumn.resize(Numy);
			}

			Fill();
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

		void Fill() {
			blockWidth = (SCREEN_WIDTH - ((float)(Numx + 1) * blocksMargin)) / (float)Numx;

			for (int i = 0; i < grid.size(); ++i) {
				for (int j = 0; j < grid[i].size(); ++j) {
					grid[i][j] = new Block();
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