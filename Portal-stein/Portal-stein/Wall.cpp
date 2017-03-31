#include "Wall.hpp"
#include "Math.hpp"

namespace ps {

	Wall::Wall(sf::Vector2f from, sf::Vector2f to, sf::Color color) : Wall(from, to, color, nullptr) {
	}

	Wall::Wall(sf::Vector2f from_, sf::Vector2f to_, sf::Color color_, sf::Texture * texture_) : from(from_), to(to_), color(color_), texture(texture_) {
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

		rt.draw(vertexArr, texture);
	}

	float Wall::getWidth() const
	{
		return norm(to - from);
	}

	void PortalWall::setPortal(portalUPtr && portal_) {
		portal = std::move(portal_);
	}

	PortalWall::PortalWall(sf::Vector2f from, sf::Vector2f to, sf::Color color) : PortalWall(from, to, color, nullptr) {
	}

	PortalWall::PortalWall(sf::Vector2f from, sf::Vector2f to, sf::Color color, sf::Texture * texture) : Wall(from, to, color, texture), portal(nullptr) {
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
		portalUPtr portal_ = std::make_unique<Door>(targetSegment_);
		result.setPortal(std::move(portal_));
		return std::move(result);
	}

	PortalWall makeWallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_) 
	{
		PortalWall result(wallFrom.getFrom(), wallFrom.getTo(), sf::Color::White);
		portalUPtr portal_ = std::make_unique<WallPortal>(wallFrom, wallTo, targetSegment_);
		result.setPortal(std::move(portal_));
		return result;
	}

}