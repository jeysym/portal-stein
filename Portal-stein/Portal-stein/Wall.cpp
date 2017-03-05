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

	ColoredWall::ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_) : Wall(from, to), drawRectangle() { 
		drawRectangle.setFillColor(color_);
	}

	void ColoredWall::draw(sf::RenderTarget & rt, const sf::FloatRect & renderArea, const sf::FloatRect & wallArea)
	{
		sf::Vector2f renderPos{ renderArea.left, renderArea.top };
		sf::Vector2f renderSize{ renderArea.width, renderArea.height };
		drawRectangle.setPosition(renderPos);
		drawRectangle.setSize(renderSize);
		rt.draw(drawRectangle);
	}

	TexturedWall::TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_) : Wall(from, to), texture(texture_), drawRectangle() {
		texture.setRepeated(true);
		drawRectangle.setTexture(&texture);
	}

	inline sf::IntRect getTextureIntRectangle(const sf::FloatRect & rectangle, const sf::Texture & texture) {
		sf::IntRect result;
		sf::Vector2u textureSize = texture.getSize();

		result.left = (int)floor(rectangle.left * textureSize.x);
		result.top = (int)floor(rectangle.top * textureSize.y);

		result.width = (int)ceil(rectangle.width * textureSize.x);
		result.height = (int)ceil(rectangle.height * textureSize.y);

		return result;
	}

	void TexturedWall::draw(sf::RenderTarget & rt, const sf::FloatRect & renderArea, const sf::FloatRect & wallArea)
	{
		sf::Vector2f renderPos{ renderArea.left, renderArea.top };
		sf::Vector2f renderSize{ renderArea.width, renderArea.height };
		drawRectangle.setPosition(renderPos);
		drawRectangle.setSize(renderSize);
		drawRectangle.setTextureRect(getTextureIntRectangle(wallArea, texture));
		rt.draw(drawRectangle);
	}


	DoorWall::DoorWall(sf::Vector2f from_, sf::Vector2f to_, std::size_t targetSegment_) : Wall(from_, to_) {
		portalUPtr portal_ = std::make_unique<Door>(targetSegment_);
		setPortal(portal_);
	}

	void DoorWall::draw(sf::RenderTarget & rt, const sf::FloatRect & renderArea, const sf::FloatRect & wallArea) {
		// DoorWall is not drawn
	}

	WallPortalWall::WallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_) : Wall(wallFrom.getFrom(), wallFrom.getTo())
	{
		portalUPtr portal_ = std::make_unique<WallPortal>(wallFrom, wallTo, targetSegment_);
		setPortal(portal_);
	}

	void WallPortalWall::draw(sf::RenderTarget & rt, const sf::FloatRect & renderArea, const sf::FloatRect & wallArea) {
		// Wall portal wall is not drawn
	}

}