#include "Sound.h"
#include "GameSettings.h"
#include <cassert>

namespace ApplesGame
{
	void LoadAndPrepareSound(SoundEntity& soundEntity, std::string fileName)
	{

		assert(soundEntity.buffer.loadFromFile(RESOURCES_PATH + fileName));
		sf::Sound createdSound;
		createdSound.setBuffer(soundEntity.buffer);
		soundEntity.sound = createdSound;
	}

	void PlaySound(sf::Sound& sound)
	{
		if (sound.getStatus() == sound.Playing)
		{
			sound.stop();
		}

		sound.play();
	}
}