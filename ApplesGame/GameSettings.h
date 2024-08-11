#pragma once
#include <string>

namespace ApplesGame
{
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";

	const short FIELD_CELL_SIZE = 20;
	const float TOP_PADDING = 60.f;
	const float ORIGIN_MULTIPLIER = 0.5f;

	// Game settings constants
	const float INITIAL_PLAYER_SIZE = 20.f;
	const float APPLE_SIZE = 20.f;
	const float RESPONSIVENESS_RADIUS = 100.f;
	const float ACTOR_SIZE = 20.f;
	const float INITIAL_SPEED = 2.f;
	const float BONUS_DURATION = 10.f;
	const float ACCELERATION = 1.f; // For each eaten apple player speed will be increased by this value
	const int NUM_APPLES = 40;
	const int NUM_STONES = 10;
	const int NUM_BONUSES = 5;
	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEGHT = 600;
	const std::string PLAYER_NAME = "Player";
}
