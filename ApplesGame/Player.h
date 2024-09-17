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
		Platform() {
			auto sh = new Rectangle();

			sh->setSize({ 180.f, 15.f });
			sh->setFillColor(sf::Color::Green);

			shape = sh;
		}

		std::vector<Line> GetLines() {
			return GetRectLines(*((Rectangle*)shape), position);
		}

		void Update(float timeDelta) override;
		void HandleInput();
		virtual Rectangle* GetShape() override { return (Rectangle*)shape; };
	};

	class Ball : public GameObject
	{
	public:
		Ball(){
			auto sh = new Circle();

			auto radius = 10.f;

			sh->setRadius(radius);
			sh->setFillColor(sf::Color::White);
			sh->setOrigin({ position.x + radius, position.y + radius });
			shape = sh;
		}

		void Update(float timeDelta) override;
		virtual Circle* GetShape() override { return (Circle*)shape; };
	};

	class Block : public GameObject
	{
	protected:
		Sprite sprite;
		short initHealth = 4;
		short health = 4;
	public:
		Block() : GameObject() {}

		Block(const Texture& texture) {
			sprite.setTexture(texture);
			

			auto sh = new Rectangle();
			sh->setFillColor(sf::Color::Green);

			shape = sh;
		}

		short& GetHealth() { return health; }
		void ApplyDamage(short value) { health -= value; }

		std::vector<Line> GetLines() {
			return GetRectLines(*((Rectangle*)shape), position);
		}

		void Draw() {
			GameObject::Draw();
			sprite.setPosition(position);
			auto rectSize = ((Rectangle*)shape)->getSize();
			sprite.setScale(GetSpriteScale(sprite, { rectSize.x, rectSize.y }));

			sf::Color color = sprite.getColor();
			auto a = ((float)initHealth - (float)health) / (float)initHealth;

			color.a = (sf::Uint8)(a * (float)255);
			sprite.setColor(color);

			Application::Instance()->GetWindow().draw(sprite);
		}

		void Move(sf::Vector2f position) {
			GameObject::Move({ position.x, position.y + TOP_PADDING });
		}
		void HandleInput() = delete;
		virtual Rectangle* GetShape() override { return (Rectangle*)shape; };
	};
}
