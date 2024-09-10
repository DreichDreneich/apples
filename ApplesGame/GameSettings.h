#pragma once
#include <string>

namespace ApplesGame
{
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";

	const float TOP_PADDING = 60.f;
	const float ORIGIN_MULTIPLIER = 0.5f;

	// Game settings constants
	const float INITIAL_SPEED = 2.f;
	const float BONUS_DURATION = 10.f;
	const float ACCELERATION = 1.f; // For each eaten apple player speed will be increased by this value
	const int NUM_X = 8;
	const int NUM_Y = 6;
	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEGHT = 600;
	const std::string PLAYER_NAME = "Player";
}
