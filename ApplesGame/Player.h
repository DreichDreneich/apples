#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"
#include "Actor.h"
#include "Application.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	enum class PlayerDirection
	{
		Up = 0,
		Right,
		Down,
		Left
	};

	class PartPosition
	{
	public:
		PartPosition() = default;
		PartPosition(Vector2i, PlayerDirection);
		PartPosition& operator=(const PartPosition& pp) {
			position = pp.position;
			direction = pp.direction;

			return *this;
		};

		Vector2i position;
		PlayerDirection direction;
	};

	struct Player
	{
		PlayerDirection direction = PlayerDirection::Up;
		sf::Texture texture;
		Vector2f position;
		PartPosition prevTailPosition;
		PlayerDirection lastDirection;

		std::vector<PartPosition> partsPositions;
		std::vector<sf::Sprite> sprite;

		bool hasBonus = false;
		float size = INITIAL_PLAYER_SIZE;
		float speed = 1.f; // Cells per second
		float nextSpeedUpdateTime = 0.f;
		float bonusTimeRemaining = BONUS_DURATION;


		void Init(const Vector2i& position, const sf::Texture& headTexture, const sf::Texture& texture);
		void HandleInput();
		void Draw();
		void Update(float timeDelta);
		void AddPart();
	};

	class Platform : public GameObject, public ScreenCollision
	{
		virtual bool HasCollision() override { return ((Rectangle*)shape)->HasCollision(); }
	public:
		Platform(const sf::Texture& texture) {
			auto sh = new Rectangle();

			sh->setSize({ 180.f, 15.f });
			sh->setFillColor(sf::Color::Green);

			shape = sh;
		}

		Line GetTop() {
			auto size = ((Rectangle*)shape)->getSize();

			return { position, { position.x + size.x, position.y } };
		}
		Line GetBottom() {
			auto size = ((Rectangle*)shape)->getSize();

			return { { position.x,  position.y + size.y }, { position.x + size.x, position.y + size.y } };
		}
		Line GetLeft() {
			auto size = ((Rectangle*)shape)->getSize();

			return { position, { position.x, position.y + size.y } };
		}
		Line GetRight() {
			auto size = ((Rectangle*)shape)->getSize();

			return { { position.x + size.x, position.y }, { position.x + size.x, position.y + size.y } };
		}

		std::vector<Line> GetLines() {
			return { GetTop(), GetRight(), GetBottom(), GetLeft() };
		}

		void Update(float timeDelta) override;
		void HandleInput();
		virtual Rectangle* GetShape() override { return (Rectangle*)shape; };
	};

	class Ball : public GameObject, public ScreenCollision
	{
		virtual bool HasCollision() override {
			auto hasCollision = ((Circle*)shape)->HasCollision();

			 
			return hasCollision;
		}

	public:
		Ball(const sf::Texture& texture) {
			auto sh = new Circle();

			auto radius = 10.f;

			sh->setRadius(radius);
			sh->setFillColor(sf::Color::White);
			sh->setOrigin({ position.x + radius, position.y + radius });
			shape = sh;
		}

		void Update(float timeDelta) override;
		virtual Circle* GetShape() override { return (Circle*)shape; };
		virtual void Bounce(const GameObject&);
	};
}
