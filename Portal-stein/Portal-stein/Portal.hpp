#pragma once
#ifndef PS_PORTAL_INCLUDED
#define PS_PORTAL_INCLUDED
#include <memory>
#include "SFML\Graphics.hpp"
#include "ObjectInScene.hpp"
#include "Geometry.hpp"

namespace ps {

	//******************************************************************
	// PORTAL CLASSES
	//******************************************************************

	// Abstract class for portals, which are things that can transport ObjectInScene.
	class Portal {
	public:

		// Object steps through portal.
		virtual void stepThrough(ObjectInScene & obj) = 0;
	};


	// Portal that only changes the segment the object is in. No change of position, or direction is applied.
	class Door : public Portal {
	private:
		std::size_t targetSegment;

	public:

		// Object steps through door.
		void stepThrough(ObjectInScene & obj);

		// Creates door to segment specified by its ID.
		Door(std::size_t targetSegment_);
	};


	// Portal that connects two 2D line segments. 
	class WallPortal : public Portal {
	private:
		std::size_t targetSegment;
		LineSegment fromWall;
		LineSegment toWall;

	public:

		// Object steps through portal.
		void stepThrough(ObjectInScene & obj);

		// Creates portal between two walls into segment specified by its ID.
		WallPortal(const LineSegment & from_, const LineSegment & to_, std::size_t targetSegment_);
	};


	using portalUPtr = std::unique_ptr<Portal>;
}
#endif // !PS_PORTAL_INCLUDED
