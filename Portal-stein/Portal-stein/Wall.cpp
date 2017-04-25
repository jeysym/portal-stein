#include "Wall.hpp"
#include "Math.hpp"

namespace ps {

	Wall::Wall(sf::Vector2f from, sf::Vector2f to, sf::Color color) : Wall(from, to, color, nullptr) {
	}

	Wall::Wall(sf::Vector2f from_, sf::Vector2f to_, sf::Color color_, std::shared_ptr<sf::Texture> texture_) : from(from_), to(to_), color(color_), texture(texture_) {
		if (texture != nullptr)
			texture->setRepeated(true);
	}

	void Wall::draw(sf::RenderTarget & rt, const WallDrawParameters & params) const {
		sf::VertexArray vertexArr( sf::PrimitiveType::Lines, 2 );

		if (texture != nullptr) {
			float texSizeX = (float)texture->getSize().x;
			float texSizeY = (float)texture->getSize().y;

			sf::Vector2f uvWallTop(params.uvWallTop);
			sf::Vector2f uvWallBottom(params.uvWallBottom);

			uvWallTop.x *= texSizeX;
			uvWallTop.y *= texSizeY;
			uvWallBottom.x *= texSizeX;
			uvWallBottom.y *= texSizeY;

			vertexArr[0] = sf::Vertex(params.scrWallTop, color, uvWallTop);
			vertexArr[1] = sf::Vertex(params.scrWallBottom, color, uvWallBottom);
		}
		else {
			vertexArr[0] = sf::Vertex(params.scrWallTop, color);
			vertexArr[1] = sf::Vertex(params.scrWallBottom, color);
		}

		rt.draw(vertexArr, texture.get());
	}

	float Wall::getWidth() const
	{
		return norm(to - from);
	}

	void PortalWall::setPortal(const portalPtr & portal_) {
		portal = portal_;
	}

	PortalWall::PortalWall(sf::Vector2f from, sf::Vector2f to, sf::Color color) : PortalWall(from, to, color, nullptr) {
	}

	PortalWall::PortalWall(sf::Vector2f from, sf::Vector2f to, sf::Color color, std::shared_ptr<sf::Texture> texture) : Wall(from, to, color, texture), portal(nullptr) {
	}

	bool PortalWall::isPortal() const
	{
		return portal.get() != nullptr;
	}

	void PortalWall::stepThrough(ObjectInScene & obj) const
	{
		if (portal) {
			portal->stepThrough(obj);
		}
	}

	bool Wall::facesRay(const Ray & ray) const
	{
		auto wallDirection = to - from;
		auto rayDirection = ray.getDirection();

		float crossProduct = cross(wallDirection, rayDirection);
		return (0 < crossProduct);
	}

	float Wall::distanceFromWall(const sf::Vector2f & point) const
	{
		sf::Vector2f r = to - from;
		float rDotR = dot(r, r);
		sf::Vector2f normal(r.y, -r.x);	// perpendicular vector to r

		// "from" is put into coordinate system center
		sf::Vector2f x = point - from;
		float rDot = dot(r, x);
		if (rDot < 0) {
			// returns distance from "from" (x == point - from)
			return norm(x);
		} 
		else if (rDot > rDotR) {
			// returns distance from "to" (x - r == point - to)
			return norm(x - r);
		}
		else {
			// return distance of point x from line segment "from"-"to"
			return (1 / norm(normal)) * dot(normal, x);
		}
	}

	bool Wall::intersect(const Ray & ray, WallIntersection & intersection) const
	{
		auto lineSegmentIntersection = ps::intersect(ray, LineSegment(from, to));
		if (lineSegmentIntersection.theyIntersect) {
			intersection.rayIntersectionDistance = lineSegmentIntersection.rayParameter;
			intersection.distanceToWallEdge = lineSegmentIntersection.lineSegmentParameter;
			return true;
		}
		else {
			return false;
		}
	}

	bool Wall::intersect(const LineSegment & lineSegment_) const
	{
		auto intersection = ps::intersect(LineSegment(from, to), lineSegment_);
		return intersection.theyIntersect;
	}

	PortalWall makeDoorWall(sf::Vector2f from_, sf::Vector2f to_, std::size_t targetSegment_) {
		PortalWall result(from_, to_, sf::Color::White);
		portalPtr portal_ = std::make_unique<Door>(targetSegment_);
		result.setPortal(std::move(portal_));
		return std::move(result);
	}

	PortalWall makeWallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_) 
	{
		PortalWall result(wallFrom.getFrom(), wallFrom.getTo(), sf::Color::White);
		portalPtr portal_ = std::make_unique<WallPortal>(wallFrom, wallTo, targetSegment_);
		result.setPortal(std::move(portal_));
		return result;
	}

}