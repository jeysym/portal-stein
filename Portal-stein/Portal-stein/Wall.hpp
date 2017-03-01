#pragma once
#ifndef PS_EDGE_INCLUDED
#define PS_EDGE_INCLUDED
#include "Portal.hpp"
#include "ObjectInScene.hpp"
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {

	struct WallIntersection;

	class Wall {
	public:
		sf::Vector2f from, to;
		float height = 1.0f;
		std::unique_ptr<Portal> portal;

		Wall(sf::Vector2f from_, sf::Vector2f to_);
		Wall(sf::Vector2f from_, sf::Vector2f to_, std::unique_ptr<Portal> & portal_);

		bool isPortal();
		void stepThrough(ObjectInScene & obj);
		bool intersect(Ray ray, WallIntersection & intersection);
		virtual void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth) = 0;
	};

	struct WallIntersection {
		Wall * wallThatWasHit;
		float rayIntersectionDistance;
		float distanceToWallEdge;
	};

	class ColoredWall : public Wall {
	public:
		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth);

		ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_);
		ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_, std::unique_ptr<Portal> & portal_);
	private:
		sf::RectangleShape drawRectangle;
	};

	class TexturedWall : public Wall {
	public:
		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth);

		TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_);
		TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_, std::unique_ptr<Portal> & portal_);
	private:
		sf::Texture texture;
		sf::RectangleShape drawRectangle;
	};

}

#endif // !PS_EDGE_INCLUDED
