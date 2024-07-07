#pragma once
#include <SFML/Audio.hpp>

namespace ApplesGame
{
	struct SoundEntity
	{
		sf::SoundBuffer buffer;
		sf::Sound sound;
	};

	void PlaySound(sf::Sound& sound);
	void LoadAndPrepareSound(SoundEntity& soundEntity, std::string fileName);
}