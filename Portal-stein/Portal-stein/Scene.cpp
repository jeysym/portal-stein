#include "Scene.hpp"

namespace ps {
	Segment::Segment(floorPtr & floor_, ceilingPtr & ceiling_) : floor(std::move(floor_)), ceiling(std::move(ceiling_)), edges() { }

	Scene::Scene() : segments() { }
}