#pragma once

#include "Player.h"
#include "GameStore.h"
#include "Enums.h"

namespace ApplesGame {
	class BonusStateBase {
	protected:
		BonusType type = BonusType::GLASS_BLOCKS;
	public:
		BonusType GetBonusType() { return type; }

		float duration = 5.f;
		float durationRemained = 5.f;

		virtual void ApplyBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball>, shared_ptr<Platform>) {
			for (auto& col : grid->GetGrid()) {
				for (auto& item : col) {
					item->SetState(new GlassBlockState());
				}
			}
		}

		virtual void RemoveBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball>, shared_ptr<Platform>) {
			for (auto& col : grid->GetGrid()) {
				for (auto& item : col) {
					item->SetPrevState();
				}
			}

			_RPTF2(_CRT_WARN, "Bonus removed x= %f\n", 0.f);
		}
	};

	class GlassBlocksBonusState {

	};

	class FireballBonusState : public BonusStateBase {
	public:
		FireballBonusState() : BonusStateBase() {
			type = BonusType::FIREBALL;
		}

		virtual void ApplyBonus(shared_ptr<BlocksGrid>, shared_ptr<Ball> ball, shared_ptr<Platform>) override {
			//ball->SetSpeed(400);
			//ball->SetTexture();

			_RPTF2(_CRT_WARN, "Bonus Fireball applied x= %f\n", 0.f);
		}

		virtual void RemoveBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball> ball, shared_ptr<Platform>) override {
			ball->SetSpeed(300);

			_RPTF2(_CRT_WARN, "Bonus Fireball removed x= %f\n", 0.f);
		}
	};

	class Bonus : public Ball {
	protected:
		shared_ptr<BonusStateBase> state = make_shared<BonusStateBase>();
		
	public:
		Bonus() {
			direction = { 0.f, 1.f };
			speed = 100.f;
		}

		Bonus(BonusStateBase& st) : Bonus() {
			state = make_shared<BonusStateBase>(st);
		}

		Bonus(const Bonus& b) : Ball(b) {}

		Bonus& operator=(const Bonus& b) {
			Bonus::operator=(b);

			return *this;
		}

		void SetDirection(const sf::Vector2f& direction) = delete;

		void Update(float timeDelta) {
			GameObject::Update(timeDelta);
		}

		virtual shared_ptr<BonusStateBase> ApplyBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball> ball, shared_ptr<Platform> platform) {
			state->ApplyBonus(grid, ball, platform);
			return state;
		}
	};
}