#include "Wall.hpp"
#include "Math.hpp"

namespace ps {
	Wall::Wall(sf::Vector2f from_, sf::Vector2f to_) : from(from_), to(to_) { }

	void Wall::setPortal(portalUPtr portal_) {
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
		sf::Vector2f solution;
		sf::Vector2f rayPosition2D{ ray.position.x, ray.position.y };
		float det = determinant(ray.direction, to - from);
		bool solveable = solveLinEq(ray.direction, from - to, from - rayPosition2D, solution);
		if (det >= 0 || solveable == false || solution.y < 0 || solution.y > 1 || solution.x <= 0) {
			return false;
		}
		else {
			intersection.rayIntersectionDistance = solution.x;
			intersection.distanceToWallEdge = norm(to - from) * solution.y;
			intersection.wallThatWasHit = this;
			return true;
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
		rt.draw(drawRectangle);
		drawRectangle.setTextureRect(sf::IntRect{ (int)(edgeDist * texture.getSize().y), 0, (int)edgeWidth, (int)texture.getSize().y});
		rt.draw(drawRectangle);
	}

	TexturedWall::TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_) : Wall(from, to), texture(texture_), drawRectangle() {
		texture.setRepeated(true);
		drawRectangle.setTexture(&texture);
	}


	DoorWall::DoorWall(sf::Vector2f from_, sf::Vector2f to_, std::size_t targetSegment_) : Wall(from_, to_) {
		setPortal(std::make_unique<Door>(targetSegment_));
	}

	void DoorWall::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth) {
		// DoorWall is not drawn
	}

	void WallPortalWall::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth) {
		// Wall portal wall is not drawn
	}

}