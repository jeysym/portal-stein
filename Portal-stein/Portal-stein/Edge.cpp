#include "Edge.hpp"
#include "Math.hpp"

namespace ps {
	Edge::Edge(sf::Vector2f from_, sf::Vector2f to_) : from(from_), to(to_) { }

	Edge::Edge(sf::Vector2f from_, sf::Vector2f to_, std::unique_ptr<Portal>& portal_) : from(from_), to(to_), portal(std::move(portal_)) { }

	bool Edge::isPortal()
	{
		return portal.get() != nullptr;
	}

	void Edge::stepThrough(ObjectInScene & obj)
	{
		if (portal) {
			portal->stepThrough(obj);
		}
	}

	bool Edge::intersect(Ray ray, EdgeIntersection & intersection)
	{
		sf::Vector2f solution;
		sf::Vector2f rayPosition2D{ ray.position.x, ray.position.y };
		float det = determinant(ray.direction, to - from);
		bool solveable = solveLinEq(ray.direction, from - to, from - rayPosition2D, solution);
		if (det >= 0 || solveable == false || solution.y < 0 || solution.y > 1 || solution.x <= 0) {
			return false;
		}
		else {
			intersection.rayDist = solution.x;
			intersection.edgeDist = norm(to - from) * solution.y;
			intersection.hitEdge = this;
			return true;
		}
	}

	void ColoredEdge::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth)
	{
		drawRectangle.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		drawRectangle.setPosition(renderArea.left, renderArea.top);
		rt.draw(drawRectangle);
	}

	ColoredEdge::ColoredEdge(sf::Vector2f from, sf::Vector2f to, sf::Color color_) : Edge(from, to), drawRectangle() { 
		drawRectangle.setFillColor(color_);
	}

	ColoredEdge::ColoredEdge(sf::Vector2f from, sf::Vector2f to, sf::Color color_, std::unique_ptr<Portal>& portal_) : Edge(from, to, portal_), drawRectangle() {
		drawRectangle.setFillColor(color_);
	}

	void TexturedEdge::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth)
	{
		drawRectangle.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		drawRectangle.setPosition(renderArea.left, renderArea.top);
		rt.draw(drawRectangle);
		drawRectangle.setTextureRect(sf::IntRect{ (int)(edgeDist * texture.getSize().y), 0, (int)edgeWidth, (int)texture.getSize().y});
		rt.draw(drawRectangle);
	}

	TexturedEdge::TexturedEdge(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_) : Edge(from, to), texture(texture_), drawRectangle() {
		texture.setRepeated(true);
		drawRectangle.setTexture(&texture);
	}

	TexturedEdge::TexturedEdge(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_, std::unique_ptr<Portal>& portal_) : Edge(from, to, portal_), texture(texture_), drawRectangle() {
		texture.setRepeated(true);
		drawRectangle.setTexture(&texture);
	}

}