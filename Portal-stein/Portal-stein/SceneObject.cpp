#include "Math.hpp"
#include "SceneObject.hpp"

namespace ps {
	SceneObject::SceneObject(sf::Vector2f position_, sf::Vector2f direction_, std::size_t segmentId_) : position(position_), direction(normalize(direction_)), segmentId(segmentId_) {}

	void SceneObject::goForward(float distance)
	{
		position += distance * direction;
	}

	void SceneObject::move(float x, float y) {
		position.x += x;
		position.y += y;
	}

	void SceneObject::rotate(float angle) {
		float x = direction.x;
		float y = direction.y;

		direction.x = cos(angle) * x - sin(angle) * y;
		direction.y = sin(angle) * x + cos(angle) * y;
	}

	void SceneObject::rotateAround(float angle, sf::Vector2f center)
	{
		move(-center.x, -center.y); // move rotation center to the origin
		rotate(angle); // rotate 
		move(center.x, center.y); // move rotation center back
	}
}