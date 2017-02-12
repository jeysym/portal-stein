#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED
#include <vector>
#include "segment.hpp"
#include "intersection.hpp"
#include "objectInScene.hpp"

namespace ps {
	class scene {
	public:
		std::vector<segment> segments;

		intersection intersect(ray r);
	};
}

#endif // !SCENE_INCLUDED
