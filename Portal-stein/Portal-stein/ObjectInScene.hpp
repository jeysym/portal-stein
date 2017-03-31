#pragma once
#ifndef PS_SCENE_OBJECT_INCLUDED
#define PS_SCENE_OBJECT_INCLUDED
#include <SFML\Graphics.hpp>

namespace ps {

	class LineSegment;

	// Base class for object inside the scene.
	class ObjectInScene {
	protected:
		sf::Vector3f position;	// Position of the object in 3D space.
		sf::Vector2f direction;	// Normalized direction of the object.
		std::size_t segmentId;	// Id of the segment, the object is in.

	public:
		ObjectInScene(const sf::Vector3f & position_, const sf::Vector2f & direction_, std::size_t segmentId_);

		// Gets the position of the object in the 3D space.
		sf::Vector3f getPosition() const;
		// Gets the 2D direction of the object.
		sf::Vector2f getDirection() const;
		// Gets ID of the segment the object is in.
		std::size_t getSegmentId() const;

		// Moves the object into another segment.
		void moveIntoSegment(std::size_t segmentId_);
		// Move the object forward in the direction it is facing by desired distance.
		void goForward(float distance);
		// Applies translation to the object's position.
		virtual void move(sf::Vector3f offset);
		// Applies translation to the object's position.
		void move(sf::Vector2f offset);
		// Moves the object up.
		void ascend(float distance);

		// Rotates the object around the z-axis, which changes its direction. The position of object is not changed.
		virtual void rotate(float angle);

		// LineSegment::mapLineSegments(const LineSegment & a, const LineSegment & b, ObjectInScene & obj) will need to acess the position of the object directly.
		friend class LineSegment;
	};

	using Ray = ObjectInScene;

	/*
	class FloatingObjectInScene : public ObjectInScene {
	private:
		float mass;

		sf::Vector3f velocity;
		float angularSpeed;

		sf::Vector3f forceEffect;
		float torqueEffect;

	public:
		FloatingObjectInScene(sf::Vector3f position, sf::Vector2f direction, std::size_t segmentId, float mass_);

		virtual void rotate(float angle) override;

		sf::Vector3f getVelocity() const;
		float getAngularSpeed() const;
		bool isMoving() const;
		void applyForce(sf::Vector3f force);
		void applyForceForward(float forceMagnitude);
		void applyForceUp(float forceMagnitude);
		void applyTorque(float torque);
		void simulate(float secondsElapsed);
	}; */
}

#endif // !PS_SCENE_OBJECT_INCLUDED
