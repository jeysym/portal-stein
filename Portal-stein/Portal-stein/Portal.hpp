#pragma once
#ifndef PS_PORTAL_INCLUDED
#define PS_PORTAL_INCLUDED
#include <memory>
#include "ObjectInScene.hpp"
#include "SFML\Graphics.hpp"
#include "Geometry.hpp"

namespace ps {
	class Portal {
	public:
		virtual void stepThrough(ObjectInScene & obj) = 0;
	};

	using portalUPtr = std::unique_ptr<Portal>;

	class Door : public Portal {
	public:
		Door(std::size_t targetSegment_);

		void stepThrough(ObjectInScene & obj);

	private:
		std::size_t targetSegment;
	};

	class WallPortal : public Portal {
	private:
		std::size_t targetSegment;
		LineSegment from;
		LineSegment to;

	public:
		WallPortal(LineSegment from_, LineSegment to_, std::size_t targetSegment_);

		void stepThrough(ObjectInScene & obj);

	};
}

#endif // !PS_PORTAL_INCLUDED
