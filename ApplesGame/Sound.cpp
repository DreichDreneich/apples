#include "Sound.h"
#include "GameSettings.h"
#include <cassert>

namespace ApplesGame
{
	SoundEntity::SoundEntity(std::string fileName)
	{
		assert(buffer.loadFromFile(RESOURCES_PATH + fileName));
		sound.setBuffer(buffer);
	}

	void SoundEntity::Play()
	{
		if (sound.getStatus() == sound.Playing)
		{
			sound.stop();
		}

		sound.play();
	}

	SoundManager::SoundManager(map<Sounds, string> soundsPaths, map<Music, string> musicPaths)
	{
		for (auto& soundsPath : soundsPaths) {
			sounds.insert({ soundsPath.first, make_unique<SoundEntity>(soundsPath.second) });
		}

		for (auto& musicPath : musicPaths) {
			music.insert({ musicPath.first, make_unique<SoundEntity>(musicPath.second) });
		}
	}

	SoundManager::~SoundManager()
	{
		for (auto& sound : sounds) {
			sound.second.reset();
		}

		for (auto& musicEntry : music) {
			musicEntry.second.reset();
		}
	}

	void SoundManager::Play(Sounds soundType)
	{
		sounds[soundType]->Play();
	}

	void SoundManager::Play(Music musicType)
	{
		music[musicType]->Play();
	}

	void SoundManager::SetLoop(Music musicType, bool isLoop)
	{
		music[musicType]->SetLoop(isLoop);
	}

	void SoundManager::SetBackgroundMusicVolume(float volume)
	{
		music[Music::Background]->SetVolume(volume);
	}

	void SoundManager::SetSoundsVolume(float volume)
	{
		for (auto& sound : sounds) {
			sound.second->SetVolume(volume);
		}
	}
}