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
		// Rotates around specific point.
		void rotateAround(float angle, sf::Vector2f center);
	};

	using Ray = SceneObject;
}

#endif // !PS_SCENE_OBJECT_INCLUDED
