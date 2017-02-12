#ifndef SEGMENT_INCLUDED
#define SEGMENT_INCLUDED
#include "edge.hpp"
#include "floorAndCeiling.hpp"
#include <vector>
#include <memory>

namespace ps {

	class segment {
	public:
		std::vector<edge_ptr> edges;
		floor_ptr floor;
		ceiling_ptr ceiling;
	};
}

#endif // !SEGMENT_INCLUDED
