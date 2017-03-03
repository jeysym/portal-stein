#include "Portal.hpp"
#include "Math.hpp"
#include "Geometry.hpp"

namespace ps {
	Door::Door(std::size_t targetSegment_) : targetSegment(targetSegment_) {	}

	void Door::stepThrough(ObjectInScene & obj)
	{
		obj.moveIntoSegment(targetSegment);
	}



	WallPortal::WallPortal(LineSegment from_, LineSegment to_, std::size_t targetSegment_) : from(from_), to(to_), targetSegment(targetSegment_) { }

	void WallPortal::stepThrough(ObjectInScene & obj)
	{
		LineSegment::mapLineSegments(from, to, obj);
		obj.moveIntoSegment(targetSegment);
	}
	
}