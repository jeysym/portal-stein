#include "Math.hpp"
#include "ObjectInScene.hpp"

namespace ps {
	ObjectInScene::ObjectInScene(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_) : position(position_), direction(normalize(direction_)), segmentId(segmentId_) {}

	void ObjectInScene::goForward(float distance) {
		position += distance * sf::Vector3f{ direction.x, direction.y, 0.0f };
	}

	void ObjectInScene::move(sf::Vector3f offset) {
		position += offset;
	}

	void ObjectInScene::rotate(float angle) {
		float x = direction.x;
		float y = direction.y;

		direction.x = cos(angle) * x - sin(angle) * y;
		direction.y = sin(angle) * x + cos(angle) * y;
	}

	void ObjectInScene::rotatePositionAround(float angle, sf::Vector2f center) {

	}

	void ObjectInScene::ascend(float distance) {
		position.z += distance;
	}
}