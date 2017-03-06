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

		// Gets width of the wall.
		float getWidth() const;
		// Assings a potral to the wall.
		void setPortal(portalUPtr & portal_);
		// Returns true, if wall is a portal.
		bool isPortal() const;
		// Object steps through wall. Has no effect if wall not a portal.
		void stepThrough(ObjectInScene & obj) const;
		// Returns true if the wall faces the ray. Returning false means this wall is not visible by that ray.
		bool facesRay(const Ray & ray);
		// Intersects the wall with a ray.
		bool intersect(const Ray & ray, WallIntersection & intersection) const;
		// Returns true if line segment intersects the wall.
		bool intersect(const LineSegment & lineSegment_) const;
		// Draws the wall on the RenderTarget.
		virtual void draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV) = 0;
	};

	struct WallIntersection {
		Wall * wallThatWasHit;
		float rayIntersectionDistance;
		float distanceToWallEdge;
	};

	class ColoredWall : public Wall {
	private:
		sf::Color color;

	public:
		ColoredWall(sf::Vector2f from, sf::Vector2f to, sf::Color color_);

		void draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV);
	};

	class TexturedWall : public Wall {
	private:
		sf::Texture texture;

	public:
		TexturedWall(sf::Vector2f from, sf::Vector2f to, sf::Texture texture_);

		void draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV);
	};

	class DoorWall : public Wall {
	public:
		DoorWall(sf::Vector2f from, sf::Vector2f to, std::size_t targetSegment);

		void draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV);
	};

	class WallPortalWall : public Wall {
	public:
		WallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_);

		void draw(sf::RenderTarget & rt, sf::Vector2f a, sf::Vector2f b, sf::Vector2f aUV, sf::Vector2f bUV);
	};
}

#endif // !PS_EDGE_INCLUDED
