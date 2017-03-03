#pragma once
#ifndef PS_EDGE_INCLUDED
#define PS_EDGE_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>
#include "Portal.hpp"
#include "ObjectInScene.hpp"
#include "Geometry.hpp"

namespace ps {

	struct WallIntersection;

	class Wall {
	private:
		LineSegment lineSegment;
		portalUPtr portal;

	public:
		Wall(sf::Vector2f from_, sf::Vector2f to_);

		void setPortal(portalUPtr & portal_);
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
		ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_);

		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth);
	
	private:
		sf::RectangleShape drawRectangle;
	};

	class TexturedWall : public Wall {
	public:
		TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_);

		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth);
	
	private:
		sf::Texture texture;
		sf::RectangleShape drawRectangle;
	};

	class DoorWall : public Wall {
	public:
		DoorWall(sf::Vector2f from, sf::Vector2f to, std::size_t targetSegment);

		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth);
	};

	class WallPortalWall : public Wall {
	public:
		WallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_);

		void draw(sf::RenderTarget & rt, sf::FloatRect renderArea, float edgeDist, float edgeWidth);
	};
}

#endif // !PS_EDGE_INCLUDED
