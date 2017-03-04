#pragma once
#ifndef PS_PORTAL_INCLUDED
#define PS_PORTAL_INCLUDED
#include <memory>
#include "ObjectInScene.hpp"
#include "SFML\Graphics.hpp"
#include "Geometry.hpp"

namespace ps {

	// Base class for portals, which are things that can transport ObjectInScene.
	class Portal {
	public:
		// Object steps through portal.
		virtual void stepThrough(ObjectInScene & obj) = 0;
	};

	using portalUPtr = std::unique_ptr<Portal>;

	// Portal that only changes the segment the object is in. No change of position, or direction is applied.
	class Door : public Portal {
	private:
		std::size_t targetSegment;

	public:
		Door(std::size_t targetSegment_);

		void stepThrough(ObjectInScene & obj);
	};

	// Portal that connects two 2D line segments. 
	class WallPortal : public Portal {
	public:
		WallPortal(LineSegment from_, LineSegment to_, std::size_t targetSegment_);

		void stepThrough(ObjectInScene & obj);

	private:
		std::size_t targetSegment;
		LineSegment from;
		LineSegment to;
	};
}

#endif // !PS_PORTAL_INCLUDED
