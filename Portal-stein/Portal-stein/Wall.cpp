#include "Wall.hpp"
#include "Math.hpp"

namespace ps {
	Wall::Wall(sf::Vector2f from_, sf::Vector2f to_) : lineSegment(from_, to_), portal() { }

	void Wall::setPortal(portalUPtr & portal_) {
		portal = std::move(portal_);
	}

	bool Wall::isPortal()
	{
		return portal.get() != nullptr;
	}

	void Wall::stepThrough(ObjectInScene & obj)
	{
		if (portal) {
			portal->stepThrough(obj);
		}
	}

	bool Wall::intersect(Ray ray, WallIntersection & intersection)
	{
		auto lineSegmentIntersection = ps::intersect(ray, lineSegment);
		if (lineSegmentIntersection.theyIntersect) {
			intersection.rayIntersectionDistance = lineSegmentIntersection.rayParameter;
			intersection.distanceToWallEdge = lineSegmentIntersection.lineSegmentParameter;
			intersection.wallThatWasHit = this;
			return true;
		}
		else {
			return false;
		}
	}

	void ColoredWall::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth)
	{
		drawRectangle.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		drawRectangle.setPosition(renderArea.left, renderArea.top);
		rt.draw(drawRectangle);
	}

	ColoredWall::ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_) : Wall(from, to), drawRectangle() { 
		drawRectangle.setFillColor(color_);
	}

	void TexturedWall::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth)
	{
		drawRectangle.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		drawRectangle.setPosition(renderArea.left, renderArea.top);
		drawRectangle.setTextureRect(sf::IntRect{ (int)(edgeDist * texture.getSize().y), 0, (int)edgeWidth, (int)texture.getSize().y});
		rt.draw(drawRectangle);
	}

	TexturedWall::TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_) : Wall(from, to), texture(texture_), drawRectangle() {
		texture.setRepeated(true);
		drawRectangle.setTexture(&texture);
	}


	DoorWall::DoorWall(sf::Vector2f from_, sf::Vector2f to_, std::size_t targetSegment_) : Wall(from_, to_) {
		portalUPtr portal_ = std::make_unique<Door>(targetSegment_);
		setPortal(portal_);
	}

	void DoorWall::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth) {
		// DoorWall is not drawn
	}

	WallPortalWall::WallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_) : Wall(wallFrom.getFrom(), wallFrom.getTo())
	{
		portalUPtr portal_ = std::make_unique<WallPortal>(wallFrom, wallTo, targetSegment_);
		setPortal(portal_);
	}

	void WallPortalWall::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth) {
		// Wall portal wall is not drawn
	}

}