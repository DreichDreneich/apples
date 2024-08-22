#pragma once
#include <SFML/Audio.hpp>
#include "map"

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
}