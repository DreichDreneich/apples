// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)
#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>

#include "Application.h"

using namespace ApplesGame;

int main()
{
	// Init random number generator
	unsigned int seed = (unsigned int)time(nullptr); // Get current time as seed. You can also use any other number to fix randomization
	srand(seed);

	auto app = new Application();

	// Game loop
	return app->Loop();
}
