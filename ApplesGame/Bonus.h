#pragma once

#include "Player.h"
#include "GameStore.h"
#include "Enums.h"

namespace ApplesGame {
	class BonusState {
		BonusType type = BonusType::GLASS_BLOCKS;
	public:
		BonusType GetBonusType() { return type; }

		float duration = 5.f;
		float durationRemained = 5.f;

		virtual void ApplyBonus(BlocksGrid& grid) {
			for (auto& col : grid.GetGrid()) {
				for (auto& item : col) {
					item->SetState(new GlassBlockState());
				}
			}
		}

		virtual void RemoveBonus(BlocksGrid& grid) {
			for (auto& col : grid.GetGrid()) {
				for (auto& item : col) {
					item->SetPrevState();
				}
			}

			_RPTF2(_CRT_WARN, "Bonus removed x= %f\n", 0.f);
		}
	};

	class Bonus : public Ball {
	protected:
		shared_ptr<BonusState> state = make_shared<BonusState>();
		
	public:
		Bonus() {
			direction = { 0.f, 1.f };
			speed = 100.f;
		}

		Bonus(Bonus& b) : Ball(b) {}

		Bonus& operator=(const Bonus& b) {
			Bonus::operator=(b);

			return *this;
		}

		void SetDirection(const sf::Vector2f& direction) = delete;

		void Update(float timeDelta) {
			GameObject::Update(timeDelta);
		}

		virtual shared_ptr<BonusState> ApplyBonus(BlocksGrid& grid) {
			state->ApplyBonus(grid);
			return state;
		}
	};
}