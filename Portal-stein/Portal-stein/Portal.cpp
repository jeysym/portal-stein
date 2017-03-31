#include "Portal.hpp"
#include "Math.hpp"

namespace ps {

	Door::Door(std::size_t targetSegment_) : targetSegment(targetSegment_) {	
	}

	void Door::stepThrough(ObjectInScene & obj)
	{
		obj.moveIntoSegment(targetSegment);
	}

	WallPortal::WallPortal(const LineSegment & from_, const LineSegment & to_, std::size_t targetSegment_) : fromWall(from_), toWall(to_), targetSegment(targetSegment_) { 
	}

	void WallPortal::stepThrough(ObjectInScene & obj)
	{
		obj.moveIntoSegment(targetSegment);
		LineSegment::mapLineSegments(fromWall, toWall, obj);
	}
	
}