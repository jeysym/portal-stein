#include <assert.h>
#include <string>
#include "Scene.hpp"

namespace ps {
	Segment::Segment(floorPtr & floor_, ceilingPtr & ceiling_) : floor(floor_), ceiling(ceiling_), edges() { }

	Scene::Scene() : segments() { }

	std::size_t Scene::addSegment(Segment segment_) {
		segments.push_back(segment_);
		return segments.size() - 1;
	}

	Segment& Scene::getSegment(std::size_t segmentId) {
		if (0 <= segmentId && segmentId < segments.size()) {
			return segments[segmentId];
		}
		else {
			// this id correspond to no segment => throw expection
			throw SegmentNotFound(segmentId);
		}
	}

	SegmentNotFound::SegmentNotFound(std::size_t segmentId_) {
		description = "Segment with id = " + std::to_string(segmentId_) + " was not found!";
	}

	const char * SegmentNotFound::what() const {
		return description.c_str();
	}
}