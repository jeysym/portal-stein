#ifndef INTERSECTION_INCLUDED
#define INTERSECTION_INCLUDED
#include "edge.hpp"

namespace ps {
	struct intersection {
		float distance;
		edge_ptr hitEdge;
		sf::Vector2f globalHitCoord;
		float localHitCoord;
	};
}

#endif // !INTERSECTION_INCLUDED
