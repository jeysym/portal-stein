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
		floorPtr floor;
		ceilingPtr ceiling;
		std::vector<std::shared_ptr<Wall>> edges;

		Segment(floorPtr & floor_, ceilingPtr & ceiling_);
	};
	
	struct Scene {
		std::vector<Segment> segments;

		Scene();
	};

}

#endif // !PS_SCENE_INCLUDED
