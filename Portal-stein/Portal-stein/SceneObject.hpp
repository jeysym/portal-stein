#pragma once
#ifndef PS_SCENE_OBJECT_INCLUDED
#define PS_SCENE_OBJECT_INCLUDED
#include <SFML\Graphics.hpp>

namespace ps {
	// Base class for object inside the scene.
	class SceneObject {
	public:
		sf::Vector2f position;	// Position of the object in 2D space.
		sf::Vector2f direction;	// Normalized direction of the object.
		std::size_t segmentId;	// Id of the segment, the object is in.

		SceneObject(sf::Vector2f position_, sf::Vector2f direction_, std::size_t segmentId_);

		// Move the object forward in the direction it is facing by desired distance.
		void goForward(float distance);
		// Applies translation to the object's position.
		void move(float x, float y);
		// Rotates the object, which changes its direction.
		void rotate(float angle);

		// Rotates position around specific point.
		void rotatePosAround(float angle, sf::Vector2f center);
	};

	// SceneObject with height information.
	class SceneHObject : public SceneObject {
	public:
		float height;	// height of the object position.

		SceneHObject(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_);
		SceneHObject(sf::Vector2f position_, float height_, sf::Vector2f direction_, std::size_t segmentId_);

		// Lifts the object up.
		void ascend(float distance);
	};

	using Ray = SceneHObject;
}

#endif // !PS_SCENE_OBJECT_INCLUDED