#include "SegmentBuilder.hpp"

namespace ps {

	SegmentBuilder::SegmentBuilder(const Floor & floor, const Ceiling & ceiling) : segment(floor, ceiling) {
		finalized = false;
	}

	void SegmentBuilder::setFloor(const Floor & floor)
	{
		if (finalized)
			throw SegmentBuilderIsFinalized();

		segment.floor = floor;
	}

	void SegmentBuilder::setCeiling(const Ceiling & ceiling)
	{
		if (finalized)
			throw SegmentBuilderIsFinalized();

		segment.ceiling = ceiling;
	}

	void SegmentBuilder::addWall(PortalWall && wall)
	{
		if (finalized)
			throw SegmentBuilderIsFinalized();

		if (segment.walls.size() == 0) {
			segment.walls.push_back(std::move(wall));
		}
		else {
			PortalWall & lastWall = *segment.walls.rbegin();

			if (lastWall.to != wall.from) {
				throw WallsAreNotConnected();
			}
			else {
				// TODO : also check convexity
				segment.walls.push_back(std::move(wall));
			}
		}
	}

	Segment && SegmentBuilder::finalize()
	{
		if (finalized)
			throw SegmentBuilderIsFinalized(); SegmentBuilderIsFinalized();

		if (segment.walls.size() == 0)
			throw EmptySegment();

		PortalWall & firstWall = *segment.walls.begin();
		PortalWall & lastWall = *segment.walls.rbegin();
		if (lastWall.to != firstWall.from) {
			throw WallsAreNotConnected();
		}

		finalized = true;
		return std::move(segment);
	}

	const char * SegmentBuilderIsFinalized::what() const noexcept
	{
		return "Cannot use SegmentBuilder after finalize() was called!";
	}

	const char * WallsAreNotConnected::what() const noexcept
	{
		return "SegmentBuilder got walls that are not connected!";
	}

	const char * EmptySegment::what() const noexcept
	{
		return "SegmentBuilder did not get any walls!";
	}

}