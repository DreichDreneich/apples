#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "GameSettings.h"
#include "Actor.h"
#include "Application.h"

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
}
