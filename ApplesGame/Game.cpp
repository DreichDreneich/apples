#include <SFML/Audio.hpp>

#include "Game.h"
#include "2dTree.h"

#include <assert.h>

#include <string>
#include <iostream>

using namespace std;


namespace ApplesGame
{
	void ToggleGameMode(State& gameState, int menuItem)
	{
		if (HasMaskFlag(gameState.gameMode, menuItem))
		{
			gameState.gameMode ^= menuItem;
		}
		else
		{
			gameState.gameMode |= menuItem;
		}
	}

	void GenerateRecordsList(State& gameState)
	{
		map<string, int>::iterator it = gameState.recordsList.begin();

		while (it != gameState.recordsList.end()) {
			it->second = rand() % 10;
			gameState.uiState.recordsList[it->first] = {};
			++it;
		}

		gameState.recordsList.insert({ PLAYER_NAME, 0 });
		gameState.uiState.recordsList.insert({ PLAYER_NAME, {} });
	}

	void InitGame(State& gameState)
	{
		gameState.gameMode = 0;
		gameState.gameState.push(GameState::MainMenu);

		// Init game resources (terminate if error)
		assert(gameState.playerTexture.loadFromFile(RESOURCES_PATH + "Pacman.png"));
		assert(gameState.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
		assert(gameState.stoneTexture.loadFromFile(RESOURCES_PATH + "Stone.png"));
		assert(gameState.bonusTexture.loadFromFile(RESOURCES_PATH + "xyz-logo.png"));
		assert(gameState.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		LoadAndPrepareSound(gameState.deathSound, "Death.wav");
		LoadAndPrepareSound(gameState.applePickSound, "AppleEat.wav");
		// Sound from sfml examples
		LoadAndPrepareSound(gameState.bonusPickSound, "ding.flac");

		GenerateRecordsList(gameState);
		InitUI(gameState.uiState, gameState.font);
		RestartGame(gameState);
	}

	void RestartGame(State& state)
	{
		// Init player
		InitPlayer(state.player, state.playerTexture);
		// Init apples
		for (int i = 0; i < NUM_APPLES; i++)
		{
			InitApple(state.apples[i], state.appleTexture);
		}

		state.applesTree = build2DTree(state.apples, 0, NUM_APPLES - 1, true);

		// Init stones
		for (int i = 0; i < NUM_STONES; i++)
		{
			InitActor(state.stones[i], state.stoneTexture);
		}

		// Init bonuses
		for (int i = 0; i < NUM_BONUSES; i++)
		{
			InitActor(state.bonuses[i], state.bonusTexture);
		}

		// Init game state
		state.numEatenApples = 0;


		//state.isGameOver = false;
		//state.gameState = {};
		//state.gameState.push(GameState::Game);


		state.timeSinceGameOver = 0.f;
		state.player.size = INITIAL_PLAYER_SIZE;
		state.player.hasBonus = false;
		state.player.bonusTimeRemaining = 10.f;
	}

	void HandleInput(State& gameState)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			gameState.player.direction = PlayerDirection::Up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			gameState.player.direction = PlayerDirection::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			gameState.player.direction = PlayerDirection::Down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			gameState.player.direction = PlayerDirection::Left;
		}
	}

	void UpdateActors(State& gameState, float timeDelta)
	{
		if (gameState.gameState.top() == GameState::GameOverMenu) {
			gameState.timeSinceGameOver += timeDelta;
			return;
		}

		// Update player
		UpdatePlayer(gameState.player, timeDelta);


		//auto nearestNeighbor = findNearestNeighbor(gameState.applesTree, gameState.player.position);

		//auto nearestApples = findPointsWithinRadius(gameState.applesTree, gameState.player.position, gameState.player.size + RESPONSIVENESS_RADIUS);

		//if (nearestNeighbor != nullptr && HasCirclesCollision(gameState.player.position, nearestNeighbor->apple->position, gameState.player.size, APPLE_SIZE))
		//{
		//	if (HasMaskFlag(gameState.gameMode, (int)GameMode::infiniteApple)) {
		//		// Move apple to a new random position
		//		InitApple(*nearestNeighbor->apple, gameState.appleTexture);
		//	}
		//	else {
		//		nearestNeighbor->apple->position = { -30.f, -30.f };
		//	}

		//	// Increase eaten apples counter
		//	gameState.numEatenApples++;

		//	if (HasMaskFlag(gameState.gameMode, (int)GameMode::withAcceleration)) {
		//		// Increase player speed
		//		gameState.player.speed += ACCELERATION;
		//	}

		//	PlaySound(gameState.applePickSound.sound);
		//}
		//
		//if (nearestNeighbor != nullptr && HasCirclesCollision(gameState.player.position, nearestNeighbor->apple->position, gameState.player.size + RESPONSIVENESS_RADIUS, APPLE_SIZE))
		//{
		//	UpdateApple(*nearestNeighbor->apple, gameState.player, timeDelta);
		//}

		for (int i = 0; i < NUM_APPLES; i++)
		{
			// Check collision with apple
			if (HasCirclesCollision(gameState.player.position, gameState.apples[i].position, gameState.player.size, APPLE_SIZE))
			{
				if (HasMaskFlag(gameState.gameMode, (int)GameMode::infiniteApple)) {
					// Move apple to a new random position
					InitApple(gameState.apples[i], gameState.appleTexture);
				}
				else {
					gameState.apples[i].position = { -30.f, -30.f };
				}
					
				// Increase eaten apples counter
				gameState.numEatenApples++;
					
				if (HasMaskFlag(gameState.gameMode, (int)GameMode::withAcceleration)) {
					// Increase player speed
					gameState.player.speed += ACCELERATION;
				}

				PlaySound(gameState.applePickSound.sound);
			}
			else if (HasCirclesCollision(gameState.player.position, gameState.apples[i].position, gameState.player.size + RESPONSIVENESS_RADIUS, APPLE_SIZE))
			{
				UpdateApple(gameState.apples[i], gameState.player, timeDelta);
			}
		}

		bool hasPlayerCollisionWithStone = false;

		for (const Actor stone : gameState.stones)
		{
			if (HasActorCollisionWithCircleShape(stone, gameState.player.position, gameState.player.size)) {
				hasPlayerCollisionWithStone = true;
				break;
			}
		}

		if (gameState.player.hasBonus) {
			gameState.player.bonusTimeRemaining -= timeDelta;

			if ((int)gameState.player.bonusTimeRemaining <= 0)
			{
				gameState.player.hasBonus = false;
				gameState.player.size = INITIAL_PLAYER_SIZE;
			}
		}

		for (int i = 0; i < NUM_BONUSES; i++)
		{
			if (HasCirclesCollision(gameState.bonuses[i].position, gameState.player.position, ACTOR_SIZE, gameState.player.size)) {
				gameState.bonuses[i].position = { -30.f, -30.f };

				if (!gameState.player.hasBonus) {
					gameState.player.size += 15;
				}

				gameState.player.hasBonus = true;
				gameState.player.bonusTimeRemaining = 10.f;
				PlaySound(gameState.bonusPickSound.sound);
				break;
			}
		}

		// Check collision with screen border
		if (HasCircleShapeCollisionWithScreenBorder(gameState.player.position, gameState.player.size) || hasPlayerCollisionWithStone)
		{
			//gameState.isGameOver = true;
			gameState.gameState.push(GameState::GameOverMenu);
			gameState.timeSinceGameOver = 0.f;

			auto prevRecord = gameState.recordsList.find(PLAYER_NAME);
			if (prevRecord->second < gameState.numEatenApples)
			{
				prevRecord->second = gameState.numEatenApples;
			}

			//auto cmp = [](std::pair<string, int>& a, std::pair<string, int>& b)
			//	{
			//		return a.second > b.second;
			//	};

			//std::sort(gameState.recordsList.begin(), gameState.recordsList.end(), cmp);

			PlaySound(gameState.deathSound.sound);
		}
	}

	void UpdateGame(State& state, float timeDelta)
	{
		if (state.gameState.top() == GameState::Game)
		{
			UpdateActors(state, timeDelta);
		}

		UpdateUI(state.uiState, state);
	}

	void DrawAppleTree(NodePtr root, sf::RenderWindow& window)
	{
		if (root == nullptr)
			return;

		DrawAppleTree(root->right, window);

		DrawApple(*root->apple, window);

		DrawAppleTree(root->left, window);
	}

	void DrawActors(State& gameState, sf::RenderWindow& window)
	{
		DrawPlayer(gameState.player, window);

		//DrawAppleTree(gameState.applesTree, window);
		for (int i = 0; i < NUM_APPLES; i++)
		{
			DrawApple(gameState.apples[i], window);
		}

		for (int i = 0; i < NUM_STONES; i++)
		{
			DrawActor(gameState.stones[i], window);
		}

		for (int i = 0; i < NUM_BONUSES; i++)
		{
			DrawActor(gameState.bonuses[i], window);
		}
	}

	void DrawGameEnvironment(State& gameState, sf::RenderWindow& window)
	{
		for (int i = 0; i < NUM_STONES; i++)
		{
			DrawActor(gameState.stones[i], window);
		}
	}

	void DrawGame(State& state, sf::RenderWindow& window)
	{
		if (state.gameState.top() == GameState::Game)
		{
			DrawActors(state, window);
			DrawGameEnvironment(state, window);
		}

		DrawUI(state, window);
	}
}