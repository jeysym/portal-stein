#include "Wall.hpp"
#include "Math.hpp"

namespace ps {
	Wall::Wall(sf::Vector2f from_, sf::Vector2f to_) : lineSegment(from_, to_), portal() { }

	float Wall::getWidth() const
	{
		return norm(lineSegment.getTo() - lineSegment.getFrom());
	}

	void Wall::setPortal(portalUPtr & portal_) {
		portal = std::move(portal_);
	}

	bool Wall::isPortal() const
	{
		return portal.get() != nullptr;
	}

	void Wall::stepThrough(ObjectInScene & obj) const
	{
		if (portal) {
			portal->stepThrough(obj);
		}
	}

	bool Wall::facesRay(const Ray & ray)
	{
		auto wallDirection = lineSegment.getTo() - lineSegment.getFrom();
		auto rayDirection = ray.getDirection();

		float crossProduct = cross(wallDirection, rayDirection);
		return (0 < crossProduct);
	}

	bool Wall::intersect(const Ray & ray, WallIntersection & intersection) const
	{
		auto lineSegmentIntersection = ps::intersect(ray, lineSegment);
		if (lineSegmentIntersection.theyIntersect) {
			intersection.rayIntersectionDistance = lineSegmentIntersection.rayParameter;
			intersection.distanceToWallEdge = lineSegmentIntersection.lineSegmentParameter;
			intersection.wallThatWasHit = (Wall*)this;
			return true;
		}
		else {
			return false;
		}
	}

	bool Wall::intersect(const LineSegment & lineSegment_) const
	{
		auto intersection = ps::intersect(lineSegment, lineSegment_);
		return intersection.theyIntersect;
	}

	ColoredWall::ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_) : Wall(from, to), color(color_) { 
	}

	void ColoredWall::draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV)
	{
		sf::VertexArray vertexArr{ sf::PrimitiveType::Lines, 2 };
		vertexArr[0] = sf::Vertex{ a, color };
		vertexArr[1] = sf::Vertex{ b, color };
		rt.draw(vertexArr);
	}

	TexturedWall::TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_) : Wall(from, to), texture(texture_) {
		texture.setRepeated(true);
	}

	void TexturedWall::draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV)
	{
		sf::VertexArray vertexArr{ sf::PrimitiveType::Lines, 2 };

		aUV.x *= texture.getSize().x;
		aUV.y *= texture.getSize().y;
		bUV.x *= texture.getSize().x;
		bUV.y *= texture.getSize().y;

		vertexArr[0] = sf::Vertex{ a, aUV };
		vertexArr[1] = sf::Vertex{ b, bUV };

		rt.draw(vertexArr, &texture);
	}


	DoorWall::DoorWall(sf::Vector2f from_, sf::Vector2f to_, std::size_t targetSegment_) : Wall(from_, to_) {
		portalUPtr portal_ = std::make_unique<Door>(targetSegment_);
		setPortal(portal_);
	}

	void DoorWall::draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV) {
		// DoorWall is not drawn
	}

	WallPortalWall::WallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_) : Wall(wallFrom.getFrom(), wallFrom.getTo())
	{
		portalUPtr portal_ = std::make_unique<WallPortal>(wallFrom, wallTo, targetSegment_);
		setPortal(portal_);
	}

	void WallPortalWall::draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV) {
		// Wall portal wall is not drawn
	}

}