#pragma once
#ifndef PS_SCENE_OBJECT_INCLUDED
#define PS_SCENE_OBJECT_INCLUDED
#include <SFML\Graphics.hpp>
#include "Transformation.hpp"

namespace ps {
	// Base class for object inside the scene.
	class ObjectInScene {
	public:
		sf::Vector3f position;	// Position of the object in 3D space.
		sf::Vector2f direction;	// Normalized direction of the object.
		std::size_t segmentId;	// Id of the segment, the object is in.

		ObjectInScene(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_);

		// Move the object forward in the direction it is facing by desired distance.
		void goForward(float distance);
		// Applies translation to the object's position.
		void move(sf::Vector3f offset);
		// Rotates the object around the z-axis, which changes its direction. The position of object is not changed.
		void rotate(float angle);
		
		void applyTransformation(AffineTransformation3 transform) {
			position = transform.transform(position);
		}
		
		// Moves the object up.
		void ascend(float distance);
	};

	using Ray = ObjectInScene;
}

#endif // !PS_SCENE_OBJECT_INCLUDED
