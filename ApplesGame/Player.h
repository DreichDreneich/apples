#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"

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
}
