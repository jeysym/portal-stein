#pragma once
#ifndef PS_EDGE_INCLUDED
#define PS_EDGE_INCLUDED
#include "Portal.hpp"
#include "SceneObject.hpp"
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {

	struct EdgeIntersection;

	class Edge {
	public:
		sf::Vector2f from, to;
		float height = 1.0f;
		std::unique_ptr<Portal> portal;

		Edge(sf::Vector2f from_, sf::Vector2f to_);
		Edge(sf::Vector2f from_, sf::Vector2f to_, std::unique_ptr<Portal> & portal_);

		bool isPortal();
		void stepThrough(SceneObject & obj);
		bool intersect(Ray ray, EdgeIntersection & intersection);
		virtual void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist) = 0;
	};

	struct EdgeIntersection {
		Edge * hitEdge;
		float rayDist;
		float edgeDist;
	};

	class ColoredEdge : public Edge {
	public:
		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist);

		ColoredEdge(sf::Vector2f from, sf::Vector2f to, sf::Color color_);
		ColoredEdge(sf::Vector2f from, sf::Vector2f to, sf::Color color_, std::unique_ptr<Portal> & portal_);
	private:
		sf::Color color;
	};

	class TexturedEdge : public Edge {
	public:
		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist);

		TexturedEdge(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_);
		TexturedEdge(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_, std::unique_ptr<Portal> & portal_);
	private:
		sf::Texture texture;
	};

}

#endif // !PS_EDGE_INCLUDED
