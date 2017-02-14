#pragma once
#ifndef PS_PORTAL_INCLUDED
#define PS_PORTAL_INCLUDED
#include "SceneObject.hpp"
#include "SFML\Graphics.hpp"

namespace ps {
	class Portal {
	public:
		virtual void stepThrough(SceneObject & obj) = 0;
	};

	class Door : public Portal {
	public:
		Door(std::size_t targetSegment_);

		void stepThrough(SceneObject & obj);

	private:
		std::size_t targetSegment;
	};

	class Teleport : public Portal {
	public:
		Teleport(sf::Vector2f move_, float rotateAngle_, sf::Vector2f rotationCentrum, std::size_t targetSegment_);
		Teleport(sf::Vector2f a1, sf::Vector2f a2, sf::Vector2f b1, sf::Vector2f b2, std::size_t targetSegment_);

		void stepThrough(SceneObject & obj);

	private:
		std::size_t targetSegment;
		sf::Vector2f move;
		sf::Vector2f rotationCentrum;
		float rotateAngle;
	};
}

#endif // !PS_PORTAL_INCLUDED
