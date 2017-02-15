#include "Edge.hpp"
#include "Math.hpp"

namespace ps {
	Edge::Edge(sf::Vector2f from_, sf::Vector2f to_) : from(from_), to(to_) { }

	Edge::Edge(sf::Vector2f from_, sf::Vector2f to_, std::unique_ptr<Portal>& portal_) : from(from_), to(to_), portal(std::move(portal_)) { }

	bool Edge::isPortal()
	{
		return portal.get() != nullptr;
	}

	void Edge::stepThrough(SceneObject & obj)
	{
		if (portal) {
			portal->stepThrough(obj);
		}
	}

	bool Edge::intersect(Ray ray, EdgeIntersection & intersection)
	{
		sf::Vector2f solution;
		float det = determinant(ray.direction, to - from);
		bool solveable = solveLinEq(ray.direction, from - to, from - ray.position, solution);
		if (det >= 0 || solveable == false || solution.y < 0 || solution.y > 1 || solution.x <= 0) {
			return false;
		}
		else {
			intersection.rayDist = solution.x;
			intersection.edgeDist = solution.y;
			intersection.hitEdge = this;
			return true;
		}
	}

	void ColoredEdge::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist)
	{
		sf::RectangleShape rect{ sf::Vector2f{renderArea.width, renderArea.height} };
		rect.setPosition(renderArea.left, renderArea.top);
		rect.setFillColor(color);
		rt.draw(rect);
	}

	ColoredEdge::ColoredEdge(sf::Vector2f from, sf::Vector2f to, sf::Color color_) : Edge(from, to), color(color_) { }

	ColoredEdge::ColoredEdge(sf::Vector2f from, sf::Vector2f to, sf::Color color_, std::unique_ptr<Portal>& portal_) : Edge(from, to, portal_), color(color_)
	{
	}

	void TexturedEdge::draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist)
	{
		sf::RectangleShape rect{ sf::Vector2f{ renderArea.width, renderArea.height } };
		rect.setPosition(renderArea.left, renderArea.top);
		rect.setFillColor(sf::Color::Magenta);	//TODO: make proper texture drawing
		rt.draw(rect);
	}

	TexturedEdge::TexturedEdge(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_) : Edge(from, to), texture(texture_) { }

	TexturedEdge::TexturedEdge(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_, std::unique_ptr<Portal>& portal_) : Edge(from, to, portal_), texture(texture_)
	{
	}

}