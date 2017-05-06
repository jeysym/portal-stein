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

	/// Abstract class for portals, which are things that can transport ObjectInScene.
	class Portal {
	public:
		/// Object steps through portal.
		virtual void stepThrough(ObjectInScene & obj) = 0;
	};


	/// Portal that only changes the segment the object is in. No change of position, or direction is applied.
	class Door : public Portal {
	private:
		std::size_t targetSegment;	///< Id of the segment the portal points to.

	public:
		/// Creates door to segment specified by its ID.
		Door(std::size_t targetSegment_);

		/// Object steps through door.
		void stepThrough(ObjectInScene & obj);
	};


	/// Portal that connects two 2D line segments. 
	class WallPortal : public Portal {
	private:
		std::size_t targetSegment;	///< Id of the segment the portal points to.
		LineSegment fromWall;		///< Wall the portal is in.
		LineSegment toWall;			///< Wall the portal leads to.

	public:
		/// Creates portal between two walls into segment specified by its ID.
		/// \param from_ Portal starts here.
		/// \param to_ Portal leads here.
		/// \param targetSegment_ Id of the segment portal leads to.
		WallPortal(const LineSegment & from_, const LineSegment & to_, std::size_t targetSegment_);

		/// Object steps through portal.
		void stepThrough(ObjectInScene & obj);
	};


	using portalPtr = std::shared_ptr<Portal>;
}
#endif // !PS_PORTAL_INCLUDED
