#pragma once
#include "Math.h"
#include "GameSettings.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct Line;

namespace ApplesGame
{
	enum class ShapeType {
		Rect, Circle
	};

	class GameObject
	{
	private:
		std::string id;

	protected:
		sf::Vector2f position = { 0.f, 0.f };
		sf::Vector2f prevPosition = { 0.f, 0.f };
		sf::Vector2f direction = { 0.f, 0.f};
		sf::Shape* shape;
		float speed = 300.f;

	public: 
		GameObject() {
			id = generate_uuid();
		}
		~GameObject() {
			delete shape;
		};

		std::string GetId() { return id; }
		sf::Vector2f& GetDirection() { return direction; }
		sf::Vector2f& GetPosition() { return position; }
		void SetSpeed(const float&);
		void SetDirection(const sf::Vector2f& direction);
		void Move(const sf::Vector2f& position) { GameObject::position = { position.x, position.y }; };
		void UndoUpdate();
		virtual sf::Shape* GetShape() { return shape; }
		virtual void Update(float timeDelta);
		virtual void Draw();
	};

	class ScreenCollision {
	public:
		// with screen border
		virtual bool HasCollisionWithWindow() = 0;
	};

	//TODO: remove
	class Circle : public sf::CircleShape {

	};

	//TODO: remove
	class Rectangle : public sf::RectangleShape, public ScreenCollision {
	public:
		bool HasCollisionWithWindow() {
			return getPosition().x < 0 || getPosition().x + getSize().x > SCREEN_WIDTH || getPosition().y < 0 || getPosition().y + getSize().y > SCREEN_HEGHT;
		};
	};
}
