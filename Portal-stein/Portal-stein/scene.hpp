#pragma once
#ifndef PS_SCENE_INCLUDED
#define PS_SCENE_INCLUDED
#include "FloorCeiling.hpp"
#include "Edge.hpp"
#include <vector>
#include <memory>

namespace ps {

	struct Segment {
		floorPtr floor;
		ceilingPtr ceiling;
		std::vector<std::shared_ptr<Edge>> edges;

		Segment(floorPtr & floor_, ceilingPtr & ceiling_);
	};
	
	struct Scene {
		std::vector<Segment> segments;

		Scene();
	};

}

#endif // !PS_SCENE_INCLUDED
