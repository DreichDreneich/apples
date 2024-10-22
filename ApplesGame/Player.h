#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"
#include "Actor.h"
#include "Application.h"
#include "Utils.h"
#include "Sound.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	class Platform : public GameObject
	{
		bool HasCollisionWithWindow() { return ((Rectangle*)shape)->HasCollisionWithWindow(); }
	public:

		Platform(Platform& b) {
			shape = new Rectangle();
			*shape = *b.shape;
			position = b.position;
			prevPosition = b.prevPosition;
			direction = b.direction;
			speed = b.speed;
		}

		Platform& operator=(const Platform& b) {
			*shape = *b.shape;
			position = b.position;
			prevPosition = b.prevPosition;
			direction = b.direction;
			speed = b.speed;

			return *this;
		}

		Platform() {
			auto sh = new Rectangle();

			sh->setSize({ 180.f, 15.f });
			sh->setFillColor(sf::Color::Green);

			shape = sh;
		}

		void Update(float timeDelta) override;
		void HandleInput();
		virtual Rectangle* GetShape() override { return (Rectangle*)shape; };
	};

	class BallStateBase {
	public:
		short speed = 300;
		Sprite sprite;

		BallStateBase() = default;

		BallStateBase(const BallStateBase& b) {
			speed = b.speed;
			sprite = b.sprite;
		}

		BallStateBase& operator=(const BallStateBase& b) {
			speed = b.speed;
			sprite = b.sprite;
		}
	};

	class FireballBallState : public BallStateBase {
	public:
		FireballBallState() : BallStateBase() {
			speed = 450;
		};
		
		FireballBallState(TexturesManager* manager) : FireballBallState() {
			sprite.setTexture(*manager->list[TextureType::FIREBALL]);
		};
	};

	class Ball : public GameObject
	{
	protected:
		shared_ptr<BallStateBase> state = make_shared<BallStateBase>();
		shared_ptr<BallStateBase> prevState = nullptr;

	public:
		Ball(const Ball& b) {
			shape = new sf::CircleShape();
			*shape = *b.shape;
			position = b.position;
			prevPosition = b.prevPosition;
			direction = b.direction;
			speed = b.speed;
			state = make_shared<BallStateBase>(*b.state);
			//sprite.setTexture(*b.sprite.getTexture());
		}

		Ball& operator=(const Ball& b) {
			*shape = *b.shape;
			position = b.position;
			prevPosition = b.prevPosition;
			direction = b.direction;
			speed = b.speed;
			//sprite.setTexture(*b.sprite.getTexture());
			state = make_shared<BallStateBase>(*b.state);

			return *this;
		}

		Ball(){
			auto sh = new sf::CircleShape();
			 
			auto radius = 10.f;

			sh->setRadius(radius);
			//sh->setFillColor(sf::Color::White);
			sh->setOrigin({ position.x + radius, position.y + radius });
			shape = sh;
		}

		void SetTexture(const Texture& texture) {
			state->sprite.setTexture(texture);
		}

		void SetState(BallStateBase& _state) {
			prevState = make_shared<BallStateBase>(*state);
			state = make_shared<BallStateBase>(_state);
			speed = state->speed;
		}

		void SetPrevState() {
			state = make_shared<BallStateBase>(*prevState);
			speed = state->speed;
			prevState.reset();
		}

		void Draw() override {
			GameObject::Draw();
			auto radius = GetShape()->getRadius();

			state->sprite.setOrigin(GetSpriteOrigin(state->sprite, { 0.5f, 0.5f }));
			state->sprite.setScale(GetSpriteScale(state->sprite, { radius * 2.f + 4.f, radius * 2.f + 4.f }));
			state->sprite.setPosition(position);
			Application::Instance()->GetWindow().draw(state->sprite);
		}

		void Update(float timeDelta) override;
		virtual Circle* GetShape() override { return (Circle*)shape; };
	};

	class BlockStateBase {
	protected:
		short health = 1;
	public:
		BlockStateBase() = default;
		BlockStateBase(short value) { health = value; }
		virtual void SetHealth(short value) { health = value; };
		virtual short GetHealth() { return health; };
		virtual void ApplyDamage(short value) {
			health -= value;
		};
	};

	class GlassBlockState : public BlockStateBase {
	public:
		GlassBlockState() : BlockStateBase() {};
		GlassBlockState(short value) : BlockStateBase(value) {};
		void ApplyDamage(short value) override {
			health = 0;
		};
	};

	enum class BlockTypes {
		BLOCK, STRONG_BLOCK,
	};

	class Block : public GameObject
	{
	protected:
		short totalHealth = 1;
		//short health = 1;
		BlockTypes type = BlockTypes::BLOCK;

		BlockStateBase* state = new BlockStateBase();

		BlockStateBase* prevState = nullptr;
	public:
		virtual BlockTypes GetType() { return type; }

		Block(Block& b) {
			auto newShape = new RectangleShape();
			newShape->setSize(b.GetShape()->getSize());
			newShape->setFillColor(b.GetShape()->getFillColor());
			newShape->setTexture(b.GetShape()->getTexture());
			shape = newShape;

			totalHealth = b.totalHealth;
			*state = *b.state;
			position = b.position;
			speed = b.speed;
			type = b.type;
		}

		Block& operator=(const Block& b) {
			*shape = *b.shape;
			totalHealth = b.totalHealth;
			state = b.state;
			position = b.position;
			speed = b.speed;
			type = b.type;

			return *this;
		}

		Block() : GameObject() {
			Block::totalHealth = 1;
			state->SetHealth(1);

			auto sh = new Rectangle();

			sh->setFillColor(sf::Color::Green);

			shape = sh;
		}

		virtual shared_ptr<Block> clone() {
			//_RPTF2(_CRT_WARN, "Block x= %f\n", 0.f);
			return std::make_shared<Block>(*this);
		}

		void SetState(BlockStateBase* nextState) {
			nextState->SetHealth(state->GetHealth());
			prevState = state;
			state = nextState;
		}

		void SetPrevState() {
			delete state;
			state = prevState;
		}

		short GetHealth() { return state->GetHealth(); }

		void ApplyDamage(short value) {
			state->ApplyDamage(value);
		}

		void Move(sf::Vector2f position) {
			GameObject::Move({ position.x, position.y + TOP_PADDING });
		}

		void HandleInput() = delete;

		virtual Rectangle* GetShape() override { return (Rectangle*)shape; };

		virtual void Draw() override { 
			GameObject::Draw();
		}
	};

	class StrongBlock : public Block {
	protected:
		Sprite sprite;

	public:

		StrongBlock(const Texture& texture) : Block() {
			Block::totalHealth = 3;
			state->SetHealth(3);
			Block::type = BlockTypes::STRONG_BLOCK;
			sprite.setTexture(texture);

			shape->setFillColor(sf::Color(210, 210, 210));
		}

		StrongBlock(StrongBlock& b): Block (b) {
			sprite.setTexture(*b.sprite.getTexture());
		}

		StrongBlock& operator=(const StrongBlock& b) {
			Block::operator=(b);
			sprite.setTexture(*b.sprite.getTexture());

			return *this;
		}

		shared_ptr<Block> clone() override {
			_RPTF2(_CRT_WARN, "Strong Block x= %f\n", 0.f);
			return std::make_shared<StrongBlock>(*this);
		}

		void Draw() override {
			Block::Draw(); 

			sprite.setPosition(position);
			auto rectSize = ((Rectangle*)shape)->getSize();
			sprite.setScale(GetSpriteScale(sprite, { rectSize.x, rectSize.y }));

			sf::Color color = sprite.getColor();
			auto a = ((float)totalHealth - (float)state->GetHealth()) / (float)totalHealth;

			color.a = (sf::Uint8)(a * (float)255);
			sprite.setColor(color);

			Application::Instance()->GetWindow().draw(sprite);
		}
	};
}
