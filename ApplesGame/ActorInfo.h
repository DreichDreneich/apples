#pragma once
#include <SFML/Graphics.hpp>
#include "Actor.h"

namespace ApplesGame {
	struct ActorInfo
	{
		sf::Texture texture;
		std::vector<GameObject> store;
		int num;
	};
}