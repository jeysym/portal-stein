#pragma once
#ifndef PS_SCENE_INCLUDED
#define PS_SCENE_INCLUDED
#include "FloorCeiling.hpp"
#include "Wall.hpp"
#include <vector>
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {

	struct Segment {
		float segmentFloorHeight = 0.0f;
		float segmentWallHeight = 1.0f;
		floorPtr floor;
		ceilingPtr ceiling;
		std::vector<std::shared_ptr<Wall>> edges;

		Segment(floorPtr & floor_, ceilingPtr & ceiling_);
	};
	
	class Scene{
	private:
		std::vector<Segment> segments;

	public:
		// Adds a new segment. Its unique id is returned.
		std::size_t addSegment(Segment segment_);

		// Gets segment by its id.
		Segment& getSegment(std::size_t segmentId);

		Scene();
	};

}

#endif // !PS_SCENE_INCLUDED
