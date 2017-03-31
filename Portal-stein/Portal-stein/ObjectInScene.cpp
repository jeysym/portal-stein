#include "ObjectInScene.hpp"
#include "Math.hpp"

namespace ps {
	ObjectInScene::ObjectInScene(const sf::Vector3f & position_, const sf::Vector2f & direction_, std::size_t segmentId_) : position(position_), direction(normalized(direction_)), segmentId(segmentId_) {}

	sf::Vector3f ObjectInScene::getPosition() const
	{
		return position;
	}

	sf::Vector2f ObjectInScene::getDirection() const
	{
		return direction;
	}

	std::size_t ObjectInScene::getSegmentId() const
	{
		return segmentId;
	}

	void ObjectInScene::moveIntoSegment(std::size_t segmentId_)
	{
		segmentId = segmentId_;
	}

	void ObjectInScene::goForward(float distance) {
		position += distance * sf::Vector3f{ direction.x, direction.y, 0.0f };
	}

	void ObjectInScene::move(sf::Vector3f offset) {
		position += offset;
	}

	void ObjectInScene::move(sf::Vector2f offset)
	{
		this->move(sf::Vector3f{ offset.x, offset.y, 0.0f });
	}

	void ObjectInScene::rotate(float angle) {
		ps::rotate(direction, angle);
		normalize(direction);
	}

	void ObjectInScene::ascend(float distance) {
		position.z += distance;
	}

}