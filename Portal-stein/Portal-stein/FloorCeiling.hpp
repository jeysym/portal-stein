#pragma once
#ifndef PS_FLOOR_CEILING_INCLUDED
#define PS_FLOOR_CEILING_INCLUDED
#include <memory>

namespace ps {
	class Floor {

	};

	class Ceiling {

	};

	using floorPtr = std::shared_ptr<Floor>;
	using ceilingPtr = std::shared_ptr<Ceiling>;
}

#endif // !PS_FLOOR_CEILING_INCLUDED
