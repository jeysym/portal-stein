#pragma once
#ifndef PS_SCENE_BUILDER_INCLUDED
#define PS_SCENE_BUILDER_INCLUDED
#include <exception>
#include "Scene.hpp"

namespace ps {

	//******************************************************************
	// EXCEPTIONS
	//******************************************************************

	class SegmentBuilderIsFinalized : std::exception {
		const char * what() const noexcept override;
	};

	class WallsAreNotConnected : std::exception {
		const char * what() const noexcept override;
	};

	class EmptySegment : std::exception {
		const char * what() const noexcept override;
	};


	//******************************************************************
	// SEGMENT BUILDER
	//******************************************************************

	class SegmentBuilder {
	private:
		Segment segment;
		bool finalized;

	public:
		SegmentBuilder(const Floor & floor, const Ceiling & ceiling);
		void setFloor(const Floor & floor);
		void setCeiling(const Ceiling & ceiling);
		void addWall(PortalWall && wall);
		Segment&& finalize();
	};

}

#endif // !PS_SCENE_BUILDER_INCLUDED
