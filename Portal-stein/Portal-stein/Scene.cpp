#include "Scene.hpp"

namespace ps {
	Segment::Segment(floorPtr & floor_, ceilingPtr & ceiling_) : floor(floor_), ceiling(ceiling_), edges() { }

	Scene::Scene() : segments() { }
}