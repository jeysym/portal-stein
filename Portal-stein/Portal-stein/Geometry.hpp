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

	/// Ray in level.
	using Ray = ObjectInScene;

	/// 2D line segment.
	class LineSegment {
	private:	
		sf::Vector2f from;
		sf::Vector2f to;

	public:
		/// Creates a line segment connecting two points in plane.
		LineSegment(const sf::Vector2f & from_, const sf::Vector2f & to_);

		/// Gets the starting point of line segment.
		sf::Vector2f getFrom() const;
		/// Gets the ending point of line segment.
		sf::Vector2f getTo() const;

		/// Maps object from one line segment to another. If line segments have different lengths direction may be mapped inaccurately.
		/// \param a Line-segment object is mapped from.
		/// \param b Line-segment object is mapped to.
		/// \param obj Object, whose position is mapped (it will be changed).
		static void mapLineSegments(const LineSegment & a, const LineSegment & b, ObjectInScene & obj);
	};

	/// Intersects a 2D ray and line segment.
	RayLineSegmentIntersection intersect(const Ray & ray, const LineSegment & lineSegment);

	/// Intersects two 2D line segments.
	LineSegmentLineSegmentIntersection intersect(const LineSegment & lineSegA, const LineSegment & lineSegB);
}

#endif // !PS_GEOMETRY_INCLUDED
