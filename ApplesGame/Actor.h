#pragma once
#include "Math.h"
#include "GameSettings.h"
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
	protected:
		sf::Vector2f position = { 0.f, 0.f };
		sf::Vector2f prevPosition = { 0.f, 0.f };
		sf::Vector2f direction = { 0.f, 0.f};
		sf::Shape* shape;
		float speed = 300.f;

	public: 
		GameObject() = default;
		~GameObject() {
			delete shape;
		};

		sf::Vector2f GetDirection() { return direction; }
		void SetSpeed(const float&);
		void SetDirection(const sf::Vector2f& direction);
		void Move(const sf::Vector2f& position) { GameObject::position = position; };
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

	class CollisionManager {
	public:
		static bool HasCollisionCircleRect(const sf::CircleShape& circle, const sf::RectangleShape& rect) {
			float circleRadius = circle.getRadius();
			float actorWidth = rect.getSize().x; 
			float actorHeight = rect.getSize().y; 
			float halfWidth = actorWidth / 2.0f;
			float halfHeight = actorHeight / 2.0f;

			// Расстояние между центром окружности и ближайшей точкой прямоугольника
			float distanceX = std::abs(circle.getPosition().x - rect.getPosition().x);
			float distanceY = std::abs(circle.getPosition().y - rect.getPosition().y);

			// Проверка на пересечение по осям X и Y
			if (distanceX > halfWidth || distanceY > halfHeight)
			{
				return false;
			}

			// Если окружность полностью находится внутри прямоугольника
			if (distanceX <= halfWidth && distanceY <= halfHeight)
			{
				return true;
			}

			// Вычисление расстояния от центра окружности до ближайшей точки прямоугольника
			double distSq = std::pow(std::max(halfWidth - distanceX, 0.f), 2) + std::pow(std::max(halfHeight - distanceY, 0.f), 2);

			// Проверка на коллизию
			return distSq <= std::pow(circleRadius, 2);
		}

		static bool HasCollisionCircleWindow(sf::Vector2f position, float radius) {
			return (position.x - radius < 0) ||
				(position.x + radius > SCREEN_WIDTH) ||
				(position.y - radius < TOP_PADDING) ||
				(position.y + radius > SCREEN_HEGHT);
		};
	};
}
