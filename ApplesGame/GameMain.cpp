// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)
#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>

#include <iostream>
#include <windows.h>

#include "Application.h"

using namespace ApplesGame;

int main()
{
	SetConsoleOutputCP(1251);

	// Init random number generator
	unsigned int seed = (unsigned int)time(nullptr); // Get current time as seed. You can also use any other number to fix randomization
	srand(seed);

	return Application::Instance()->Loop();
}
