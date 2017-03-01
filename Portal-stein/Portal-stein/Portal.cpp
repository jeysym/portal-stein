#include "Portal.hpp"
#include "Math.hpp"

namespace ps {
	Door::Door(std::size_t targetSegment_) : targetSegment(targetSegment_) {	}

	void Door::stepThrough(ObjectInScene & obj)
	{
		obj.segmentId = targetSegment;
	}

	Teleport::Teleport(sf::Vector2f move_, float rotateAngle_, sf::Vector2f rotationCentrum_ ,std::size_t targetSegment_) : move(move_), rotateAngle(rotateAngle_), targetSegment(targetSegment_), rotationCentrum(rotationCentrum_) { }

	Teleport::Teleport(sf::Vector2f a1, sf::Vector2f a2, sf::Vector2f b1, sf::Vector2f b2, std::size_t targetSegment_) : targetSegment(targetSegment_)
	{
		move = b1 - a1;
		sf::Vector2f x = a2 - a1;
		sf::Vector2f y = b2 - b1;
		rotateAngle = angle(x, y);
		rotationCentrum = a1;
	}

	void Teleport::stepThrough(ObjectInScene & obj)
	{
		// TODO : make teleport work
	}
}