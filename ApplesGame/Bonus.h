#pragma once

#include "Player.h"
#include "GameStore.h"
#include "Enums.h"

namespace ApplesGame {
	class BonusStateBase {
	protected:
		BonusType type = BonusType::GLASS_BLOCKS;
		BallStateBase* ballState;

	public:
		BonusStateBase() = default;
		BallStateBase* GetBallState() { return ballState; }

		BonusType GetBonusType() { return type; }

		float duration = 10.f;
		float durationRemained = 10.f;

		virtual void ApplyBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball>, shared_ptr<Platform>) {
		}

		virtual void RemoveBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball>, shared_ptr<Platform>) {
		}
	};

	class GlassBlocksBonusState : public BonusStateBase {
	public:
		GlassBlocksBonusState(TexturesManager* tm) : BonusStateBase() {
			type = BonusType::GLASS_BLOCKS;
			ballState = new GlassBlocksBonusBallState(tm);
		};

		virtual void ApplyBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball>, shared_ptr<Platform>) override {
			for (auto& col : grid->GetGrid()) {
				for (auto& item : col) {
					item->SetState(new GlassBlockState());
				}
			}

			_RPTF2(_CRT_WARN, "Bonus GlassBlocks applied x= %f\n", 0.f);
		}

		virtual void RemoveBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball>, shared_ptr<Platform>) override {
			for (auto& col : grid->GetGrid()) {
				for (auto& item : col) {
					item->SetPrevState();
				}
			}

			_RPTF2(_CRT_WARN, "Bonus GlassBlocks removed x= %f\n", 0.f);
		}
	};

	class FireballBonusState : public BonusStateBase {
		TexturesManager* texturesManager;
	public:
		FireballBonusState(TexturesManager* tm) : BonusStateBase() {
			type = BonusType::FIREBALL;
			ballState = new FireballBonusBallState(tm);
			texturesManager = tm;
		}

		virtual void ApplyBonus(shared_ptr<BlocksGrid>, shared_ptr<Ball> ball, shared_ptr<Platform>) override {
			auto state = FireballBallState(texturesManager);
			ball->SetState(state);

			_RPTF2(_CRT_WARN, "Bonus Fireball applied x= %f\n", 0.f);
		}

		virtual void RemoveBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball> ball, shared_ptr<Platform>) override {
			ball->SetPrevState();

			_RPTF2(_CRT_WARN, "Bonus Fireball removed x= %f\n", 0.f);
		}
	};

	class FastPlatformBonusState : public BonusStateBase {
	public:
		FastPlatformBonusState(TexturesManager* tm) : BonusStateBase() {
			type = BonusType::FAST_PLATFORM;
			ballState = new FastPlatformBonusBallState(tm);
		};

		virtual void ApplyBonus(shared_ptr<BlocksGrid>, shared_ptr<Ball>, shared_ptr<Platform> platform) override {
			platform->SetSpeed(600.f);
			platform->GetShape()->setFillColor(Color::Red);

			_RPTF2(_CRT_WARN, "Bonus FastPlatform applied x= %f\n", 0.f);
		}

		virtual void RemoveBonus(shared_ptr<BlocksGrid>, shared_ptr<Ball>, shared_ptr<Platform> platform) override {
			platform->SetSpeed(450.f);
			platform->GetShape()->setFillColor(Color::Green);


			_RPTF2(_CRT_WARN, "Bonus FastPlatform removed x= %f\n", 0.f);
		}
	};

	class Bonus : public Ball {
	protected:
		BonusStateBase* bonusState;
		
	public:
		Bonus() {
			direction = { 0.f, 1.f };
			speed = 100.f;
		}

		Bonus(BonusStateBase* st) : Bonus() {
			bonusState = st;
			auto ballState = st->GetBallState();
			SetState(*ballState);
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

		virtual BonusStateBase* ApplyBonus(shared_ptr<BlocksGrid> grid, shared_ptr<Ball> ball, shared_ptr<Platform> platform) {
			bonusState->ApplyBonus(grid, ball, platform);
			return bonusState;
		}
	};
}