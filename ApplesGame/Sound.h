#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cassert>
#include "map"
#include "GameSettings.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	class SoundEntity
	{
	private:
		SoundBuffer buffer;
		Sound sound;
	public:
		//SoundEntity() = default;
		SoundEntity(string fileName);
		void Play();
		void SetLoop(bool isLoop) { sound.setLoop(isLoop); }
		void SetVolume(float volume) { sound.setVolume(volume); }
	};

	enum class Sounds {
		DeathSound, BonusPickSound, ApplePickSound,
	};

	enum class Music {
		Background,
	};

	class SoundManager {
		map<Sounds, unique_ptr<SoundEntity>> sounds;
		map<Music, unique_ptr<SoundEntity>> music;

	public:
		SoundManager(map<Sounds, string> sounds, map<Music, string> music);
		~SoundManager();
		void Play(Sounds);
		void Play(Music);
		void SetLoop(Music musicType, bool isLoop);
		void SetBackgroundMusicVolume(float);
		void SetSoundsVolume(float);
	};

	enum class TextureType {
		BLOCK, FIREBALL
	};

	class TexturesManager {
	public:
		TexturesManager(map<TextureType, string> textures) {
			for (auto& textureInfo : textures) {
				Texture texture;
				assert(texture.loadFromFile(RESOURCES_PATH + textureInfo.second));
				list.insert({ textureInfo.first, make_unique<Texture>(texture) });
			}
		}
		map<TextureType, unique_ptr<sf::Texture>> list;
	};
}