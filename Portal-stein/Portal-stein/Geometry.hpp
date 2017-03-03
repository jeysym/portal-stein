#pragma once
#ifndef PS_GEOMETRY_INCLUDED
#define PS_GEOMETRY_INCLUDED
#include "ObjectInScene.hpp"

namespace ps {

	struct RayLineSegmentIntersection {
		bool theyIntersect;
		float rayParameter;
		float lineSegmentParameter;
	};

	struct LineSegmentLineSegmentIntersection {
		bool theyIntersect;
		float firstParameter;
		float secondParameter;
	};

	using Ray = ObjectInScene;

	class LineSegment {
	private:	
		sf::Vector2f from;
		sf::Vector2f to;

	public:
		LineSegment(sf::Vector2f from_, sf::Vector2f to_);

		sf::Vector2f getFrom() const;
		sf::Vector2f getTo() const;

		// Maps object from one line segment to another.
		static void mapLineSegments(const LineSegment & a, const LineSegment & b, ObjectInScene & obj);
	};


	RayLineSegmentIntersection intersect(const Ray & ray, const LineSegment & lineSegment);

	LineSegmentLineSegmentIntersection intersect(const LineSegment & lineSegA, const LineSegment & lineSegB);
}

#endif // !PS_GEOMETRY_INCLUDED
