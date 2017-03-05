#pragma once
#ifndef PS_SCENE_INCLUDED
#define PS_SCENE_INCLUDED
#include <vector>
#include <memory>
#include <exception>
#include <SFML\Graphics.hpp>
#include "FloorCeiling.hpp"
#include "Wall.hpp"

namespace ps {

	// Exception that is thrown when trying to get segment from scene with invalid segment ID.
	class SegmentNotFound : public std::exception {
	private:
		std::string description;

	public:
		SegmentNotFound(std::size_t segmentId_);

		virtual const char * what() const;
	};


	struct Segment {
		Segment(floorPtr & floor_, ceilingPtr & ceiling_);

		float segmentFloorHeight = 0.0f;
		float segmentWallHeight = 1.0f;
		floorPtr floor;
		ceilingPtr ceiling;
		std::vector<std::shared_ptr<Wall>> edges;
	};
	
	class Scene {
	private:
		std::vector<Segment> segments;

	public:
		Scene();

		// Adds a new segment. Its unique id is returned.
		std::size_t addSegment(Segment segment_);
		// Gets segment by its id.
		Segment& getSegment(std::size_t segmentId);
	};

	using scenePtr = std::shared_ptr<Scene>;

}

#endif // !PS_SCENE_INCLUDED
