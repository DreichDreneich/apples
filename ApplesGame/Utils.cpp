#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "GameSettings.h"

namespace ApplesGame {
	bool CollisionManager::HasCollisionCircleRect(const sf::CircleShape& circle, const sf::RectangleShape& rect) {
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

	bool CollisionManager::HasCollisionCircleWindow(sf::Vector2f position, float radius) {
		return (position.x - radius < 0) ||
			(position.x + radius > SCREEN_WIDTH) ||
			(position.y - radius < TOP_PADDING) ||
			(position.y + radius > SCREEN_HEGHT);
	};

	std::vector<sf::Vector2f> findIntersectionCircleLine(sf::Vector2f circleCenter, float r, sf::Vector2f point1, sf::Vector2f point2) {
		std::vector<sf::Vector2f> intersections;

		// Вектор отрезка
		auto dx = point2.x - point1.x;
		auto dy = point2.y - point1.y;

		// Коэффициенты квадратного уравнения для нахождения параметра t
		auto A = dx * dx + dy * dy;
		auto B = 2 * (dx * (point1.x - circleCenter.x) + dy * (point1.y - circleCenter.y));
		auto C = (point1.x - circleCenter.x) * (point1.x - circleCenter.x) + (point1.y - circleCenter.y) * (point1.y - circleCenter.y) - r * r;

		// Дискриминант
		auto D = B * B - 4 * A * C;

		if (D < 0) {
			return intersections;  // Пересечений нет
		}

		// Если дискриминант положительный или нулевой, есть одно или два решения
		auto sqrtD = sqrt(D);
		auto t1 = (-B + sqrtD) / (2 * A);
		auto t2 = (-B - sqrtD) / (2 * A);

		if (t1 >= 0 && t1 <= 1) {
			auto x = point1.x + t1 * dx;
			auto y = point1.y + t1 * dy;
			intersections.push_back({ x, y });
		}

		if (t2 >= 0 && t2 <= 1 && t1 != t2) {
			auto x = point1.x + t2 * dx;
			auto y = point1.y + t2 * dy;
			intersections.push_back({ x, y });
		}

		return intersections;
	}

	// Функция для вычисления скалярного произведения двух векторов
	float dotProduct(float x1, float y1, float x2, float y2) {
		return x1 * x2 + y1 * y2;
	}

	// Функция для вычисления длины (модуля) вектора
	float vectorMagnitude(float x, float y) {
		return sqrt(x * x + y * y);
	}

	void normalize(float& x, float& y) {
		auto mag = vectorMagnitude(x, y);
		if (mag > 0) {
			x /= mag;
			y /= mag;
		}
	}

	double findCollisionAngle(sf::Vector2f circleCenter, float r, sf::Vector2f direction, sf::Vector2f point1, sf::Vector2f point2) {
		// Найти точки пересечения
		auto intersections = findIntersectionCircleLine(circleCenter, r, point1, point2);

		if (intersections.empty()) {
			// Если пересечений нет, столкновения нет
			return -1;  // Специальное значение для обозначения отсутствия столкновения
		}

		// Вектор отрезка
		auto segmentX = point2.x - point1.x;
		auto segmentY = point2.y - point1.y;

		// Нормальный вектор к отрезку
		auto normalX = -segmentY;
		auto normalY = segmentX;

		// Скалярное произведение вектора движения и нормали
		auto dot = dotProduct(direction.x, direction.y, normalX, normalY);

		// Модули (длины) вектора движения и нормали
		auto magV = vectorMagnitude(direction.x, direction.y);
		auto magN = vectorMagnitude(normalX, normalY);

		// Косинус угла между вектором движения и нормалью
		auto cosTheta = dot / (magV * magN);

		// Угол в радианах (через арккосинус)
		auto angleRadians = acos(cosTheta);

		// Угол в градусах
		auto angleDegrees = angleRadians * 180.0f / (float)M_PI;

		return angleDegrees;
	}

	sf::Vector2f reflectVector(sf::Vector2f circleCenter, float r, sf::Vector2f direction, sf::Vector2f point1, sf::Vector2f point2) {
		std::vector<sf::Vector2f> intersections = findIntersectionCircleLine(circleCenter, r, point1, point2);

		if (intersections.empty()) {
			return direction;  // Если пересечений нет, возвращаем исходный вектор
		}

		auto segmentX = point2.x - point1.x;
		auto segmentY = point2.y - point1.y;

		// Нормальный вектор к отрезку
		auto normalX = -segmentY;
		auto normalY = segmentX;

		// Нормализуем нормальный вектор
		normalize(normalX, normalY);

		// Скалярное произведение вектора движения и нормали
		auto dot = dotProduct(direction.x, direction.y, normalX, normalY);

		// Рассчитываем отражённый вектор
		auto reflectedVx = direction.x - 2.f * dot * normalX;
		auto reflectedVy = direction.y - 2.f * dot * normalY;

		return { reflectedVx, reflectedVy };
	}

	std::vector<Line>::const_iterator findIntersectionCircleRectangle(sf::Vector2f circleCenter, float r, const std::vector<Line>& lines) {
		auto lineIt = lines.begin();

		for (lineIt; lineIt < lines.end(); ++lineIt) {
			auto line_intersections = findIntersectionCircleLine(
				circleCenter, r, lineIt->p1, lineIt->p2
			);

			if (line_intersections.size() > 0) {
				return lineIt;
			}
		}

		return lineIt;
	}

	std::vector<Line> GetRectLines(const sf::RectangleShape& rect, sf::Vector2f position) {
		auto size = rect.getSize();

		return {
			{ position, { position.x + size.x, position.y } },
			{ { position.x,  position.y + size.y }, { position.x + size.x, position.y + size.y } },
			{ position, { position.x, position.y + size.y } },
			{ { position.x + size.x, position.y }, { position.x + size.x, position.y + size.y } }
		};
	}
}