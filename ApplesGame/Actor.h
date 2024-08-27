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
		float speed = 300.f;
		sf::Shape* shape;

	public: 
		GameObject() = default;

		virtual sf::Shape* GetShape() { return shape; }
		void SetSpeed(const float&);
		sf::Vector2f GetDirection() { return direction; }
		virtual void Draw();
		void SetDirection(const sf::Vector2f& direction);
		void Move(const sf::Vector2f& position)
		{
			GameObject::position = position;
		};
		virtual void Update(float timeDelta);
		void UndoUpdate();
	};

	class ScreenCollision {
	public:
		// with screen border
		virtual bool HasCollision() = 0;
	};

	class Circle : public sf::CircleShape, public ScreenCollision {
	public:
		bool HasCollision() {
			auto position = getPosition();

			return (position.x - getRadius() < 0) ||
				(position.x + getRadius() > SCREEN_WIDTH) ||
				(position.y - getRadius() < 0) ||
				(position.y + getRadius() > SCREEN_HEGHT);
		};
	};

	class Rectangle : public sf::RectangleShape, public ScreenCollision {
	public:
		bool HasCollision() {
			return getPosition().x < 0 || getPosition().x + getSize().x > SCREEN_WIDTH || getPosition().y < 0 || getPosition().y + getSize().y > SCREEN_HEGHT;
		};
	};

	class CollisionManager {
	public:
		static bool HasCollision(const sf::CircleShape& circle, const sf::RectangleShape& rect) {
			float circleRadius = circle.getRadius();
			float actorWidth = rect.getSize().x; // Ширина актера
			float actorHeight = rect.getSize().y; // Высота актера
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
	};
}
