#pragma once
#ifndef PS_SCENE_OBJECT_INCLUDED
#define PS_SCENE_OBJECT_INCLUDED
#include <SFML\Graphics.hpp>

namespace ps {

	class LineSegment;

	/// Base class for object inside the scene.
	class ObjectInScene {
	protected:
		sf::Vector3f position;	///< Position of the object in 3D space.
		sf::Vector2f direction;	///< Normalized direction of the object.
		std::size_t segmentId;	///< Id of the segment, the object is in.

	public:
		/// Creates object in scene.
		/// \param position_ Position of the object.
		/// \param direction_ Direction the object is facing.
		/// \param segmentId_ Id of the segment the object is in.
		ObjectInScene(const sf::Vector3f & position_, const sf::Vector2f & direction_, std::size_t segmentId_);

		/// Gets the position of the object in the 3D space.
		sf::Vector3f getPosition() const;
		/// Gets the 2D direction of the object.
		sf::Vector2f getDirection() const;
		/// Gets ID of the segment the object is in.
		std::size_t getSegmentId() const;

		/// Moves the object into another segment.
		void moveIntoSegment(std::size_t segmentId_);
		/// Move the object forward in the direction it is facing by desired distance.
		void goForward(float distance);
		/// Applies translation to the object's position.
		virtual void move(sf::Vector3f offset);
		/// Applies translation to the object's position.
		void move(sf::Vector2f offset);
		/// Moves the object up.
		void ascend(float distance);

		/// Rotates the object around the z-axis, which changes its direction. The position of object is not changed.
		virtual void rotate(float angle);

		/// LineSegment::mapLineSegments(const LineSegment & a, const LineSegment & b, ObjectInScene & obj) will need to acess the position of the object directly.
		friend class LineSegment;
	};

	using Ray = ObjectInScene;
}

#endif // !PS_SCENE_OBJECT_INCLUDED
