#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"

namespace ApplesGame
{
	enum class PlayerDirection
	{
		Up = 0,
		Right,
		Down,
		Left
	};

	struct Player
	{
		Position position;
		float speed = 0.f; // Pixels per second
		PlayerDirection direction = PlayerDirection::Up;
		sf::Sprite sprite;
		float size = INITIAL_PLAYER_SIZE;
		bool hasBonus = false;
		float bonusTimeRemaining = BONUS_DURATION;
	};

	void InitPlayer(Player& player, const sf::Texture& texture);
	void UpdatePlayer(Player& player, float timeDelta);

	// TODO: move to separate file
	bool HasCircleShapeCollisionWithScreenBorder(const Position& position, float size);
	bool HasCirclesCollision(const Position& circle1, const Position& circle2, float circle1Size, float circle2Size);

	void DrawPlayer(Player& player, sf::RenderWindow& window);
}
