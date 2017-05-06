#pragma once
#ifndef PS_SCENE_BUILDER_INCLUDED
#define PS_SCENE_BUILDER_INCLUDED
#include <exception>
#include "Scene.hpp"

namespace ps {

	//******************************************************************
	// EXCEPTIONS
	//******************************************************************

	/// Exception signalling that SegmentBuilder is used after calling finallize().
	class SegmentBuilderIsFinalized : std::exception {
		const char * what() const noexcept override;
	};

	/// Exception signalling that walls added to SegmentBuilder are not adjacent.
	class WallsAreNotConnected : std::exception {
		const char * what() const noexcept override;
	};

	/// Exception signalling that SegmentBuilder received no walls.
	class EmptySegment : std::exception {
		const char * what() const noexcept override;
	};


	//******************************************************************
	// SEGMENT BUILDER
	//******************************************************************

	/// Class used for building segments. This class makes sure that built segment is correct (walls are adjacent, they form a convex polygon).
	class SegmentBuilder {
	private:
		Segment segment;
		bool finalized;

	public:
		SegmentBuilder(const Floor & floor, const Ceiling & ceiling);
		void setFloor(const Floor & floor);
		void setCeiling(const Ceiling & ceiling);
		void addWall(PortalWall && wall);
		void setFinish(bool finish);
		/// Ends segment building and returns loaded segment. After this call no other methods of this class shall be called.
		Segment&& finalize();
	};

}

#endif // !PS_SCENE_BUILDER_INCLUDED
