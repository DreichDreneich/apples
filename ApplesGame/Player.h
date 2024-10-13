#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"
#include "Actor.h"
#include "Application.h"
#include "Utils.h"

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

	class Ball : public GameObject
	{
	public:

		Ball(Ball& b) {
			shape = new sf::CircleShape();
			*shape = *b.shape;
			position = b.position;
			prevPosition = b.prevPosition;
			direction = b.direction;
			speed = b.speed;
		}

		Ball& operator=(const Ball& b) {
			*shape = *b.shape;
			position = b.position;
			prevPosition = b.prevPosition;
			direction = b.direction;
			speed = b.speed;

			return *this;
		}

		Ball(){
			auto sh = new sf::CircleShape();

			auto radius = 10.f;

			sh->setRadius(radius);
			sh->setFillColor(sf::Color::White);
			sh->setOrigin({ position.x + radius, position.y + radius });
			shape = sh;
		}

		void Update(float timeDelta) override;
		virtual Circle* GetShape() override { return (Circle*)shape; };
	};

	enum class BlockTypes {
		BLOCK, STRONG_BLOCK,
	};

	class Block : public GameObject
	{
	protected:
		short totalHealth = 1;
		short health = 1;
		BlockTypes type = BlockTypes::BLOCK;

	public:
		virtual BlockTypes GetType() { return type; }

		Block(Block& b) {
			auto newShape = new RectangleShape();
			newShape->setSize(b.GetShape()->getSize());
			newShape->setFillColor(b.GetShape()->getFillColor());
			newShape->setTexture(b.GetShape()->getTexture());
			shape = newShape;

			totalHealth = b.totalHealth;
			health = b.health;
			position = b.position;
			speed = b.speed;
			type = b.type;
		}

		Block& operator=(const Block& b) {
			*shape = *b.shape;
			totalHealth = b.totalHealth;
			health = b.health;
			position = b.position;
			speed = b.speed;
			type = b.type;

			return *this;
		}

		Block() : GameObject() {
			Block::totalHealth = health;
			Block::health = health;

			auto sh = new Rectangle();

			sh->setFillColor(sf::Color::Green);

			shape = sh;
		}

		virtual shared_ptr<Block> clone() {
			//return std::make_shared<Block>(*this);
			_RPTF2(_CRT_WARN, "Block x= %f\n", 0.f);
			return std::make_shared<Block>(*this);
		}

		short& GetHealth() { return health; }

		void ApplyDamage(short value) { health -= value; }

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
			Block::health = 3;
			Block::type = BlockTypes::STRONG_BLOCK;
			sprite.setTexture(texture);

			shape->setFillColor(sf::Color(210, 210, 210));
		}

		StrongBlock(StrongBlock& b) {
			auto newShape = new RectangleShape();
			newShape->setSize(b.GetShape()->getSize());
			newShape->setFillColor(b.GetShape()->getFillColor());
			newShape->setTexture(b.GetShape()->getTexture());
			shape = newShape;

			totalHealth = b.totalHealth;
			health = b.health;
			position = b.position;
			speed = b.speed;
			type = b.type;

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
			auto a = ((float)totalHealth - (float)health) / (float)totalHealth;

			color.a = (sf::Uint8)(a * (float)255);
			sprite.setColor(color);

			Application::Instance()->GetWindow().draw(sprite);
		}
	};
}
