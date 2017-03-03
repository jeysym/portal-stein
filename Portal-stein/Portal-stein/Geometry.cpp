#include "Geometry.hpp"
#include "Solve.hpp"

namespace ps {
	LineSegment::LineSegment(sf::Vector2f from_, sf::Vector2f to_) : from(from_), to(to_) { }

	sf::Vector2f LineSegment::getFrom() const
	{
		return from;
	}

	sf::Vector2f LineSegment::getTo() const
	{
		return to;
	}

	void LineSegment::mapLineSegments(const LineSegment & a, const LineSegment & b, ObjectInScene & obj)
	{
		// TODO : doesn't handle mapping of the direction when line segment is scaled
		auto aDirection = a.to - a.from;
		auto bDirection = b.to - b.from;
		float aSegmentLength = norm(aDirection);
		float bSegmentLength = norm(bDirection);
		float angleFromAtoB = angleBetween(aDirection, bDirection);
		float angleOfA = angleBetween(aDirection, sf::Vector2f{ 1.0f, 0.0f });

		sf::Vector2f position2D{ obj.position.x, obj.position.y };

		// TODO : this all looks awful, make it pretty
		position2D -= a.from;					// move a.from to origin

		ps::rotate(position2D, -angleOfA);
		position2D.x *= (bSegmentLength / aSegmentLength);
		ps::rotate(position2D, angleOfA);

		ps::rotate(position2D, -angleFromAtoB);	// rotate by desired angle
		position2D += b.from;					// move origin to b.from

		auto offset = position2D - sf::Vector2f{ obj.position.x, obj.position.y };
		obj.rotate(-angleFromAtoB);
		obj.move(offset);
	}

	RayLineSegmentIntersection intersect(const Ray & ray, const LineSegment & lineSegment)
	{
		RayLineSegmentIntersection result;

		auto rayPosition = ray.getPosition();
		sf::Vector2f rayPosition2D = sf::Vector2f{ rayPosition.x, rayPosition.y };
		sf::Vector2f lineSegDirection = lineSegment.getTo() - lineSegment.getFrom();
		
		Matrix2<float> matrix{ ray.getDirection(), -1.0f * lineSegDirection };
		sf::Vector2f b = lineSegment.getFrom() - rayPosition2D;

		auto solutionObject = linearSolve(matrix, b);
		auto & solution = solutionObject.solution;
		bool intersectionInFrontOfRay = (0.0f <= solution.x);
		bool intersectionInLineSegment = (0.0f <= solution.y && solution.y <= 1.0f);
		if (solutionObject.solveable && intersectionInFrontOfRay && intersectionInLineSegment) {
			result.theyIntersect = true;
			result.rayParameter = solutionObject.solution.x;
			result.lineSegmentParameter = solutionObject.solution.y;
			return result;
		}
		else {
			result.theyIntersect = false;
			return result;
		}
	}

	LineSegmentLineSegmentIntersection intersect(const LineSegment & lineSegA, const LineSegment & lineSegB)
	{
		LineSegmentLineSegmentIntersection result;

		auto directionA = lineSegA.getTo() - lineSegA.getFrom();
		auto directionB = lineSegB.getTo() - lineSegB.getFrom();

		Matrix2<float> matrix{ directionA, -1.0f * directionB };
		sf::Vector2f b = lineSegB.getFrom() - lineSegA.getFrom();

		auto solutionObject = linearSolve(matrix, b);
		
		auto & solution = solutionObject.solution;
		bool aInRange = (0.0f <= solution.x && solution.x <= 1.0f);
		bool bInRange = (0.0f <= solution.y && solution.y <= 1.0f);
		if (solutionObject.solveable && aInRange && bInRange) {
			result.theyIntersect = true;
			result.firstParameter = solution.x;
			result.secondParameter = solution.y;
			return result;
		}
		else {
			result.theyIntersect = false;
			return result;
		}
	}
}