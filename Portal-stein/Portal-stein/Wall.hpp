#pragma once
#ifndef PS_WALL_INCLUDED
#define PS_WALL_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>
#include "Portal.hpp"
#include "ObjectInScene.hpp"
#include "Geometry.hpp"

namespace ps {

	struct WallDrawParameters {
		sf::Vector2f scrWallTop;		// Screen coordinate of wall top
		sf::Vector2f scrWallBottom;		// Screen coordinate of wall bottom
		sf::Vector2f uvWallTop;			// Texture coordinate of wall top
		sf::Vector2f uvWallBottom;		// Texture coordinate of wall bottom
	};

	struct WallIntersection {
		float rayIntersectionDistance;	// Distance of from ray origin to hit point
		float distanceToWallEdge;		// Distance from Wall edge to hit point
	};



	//************************************************************************
	// WALL CLASSES
	//************************************************************************

	// Class that represents wall, that has color and (optional) texture.
	class Wall {
	private:
		sf::Color color;
		std::shared_ptr<sf::Texture> texture;

	public:
		sf::Vector2f from;
		sf::Vector2f to;

		// Creates a colored wall.
		Wall(sf::Vector2f from, sf::Vector2f to, sf::Color color);
		// Creates a wall with color + texture. 
		Wall(sf::Vector2f from, sf::Vector2f to, sf::Color color, std::shared_ptr<sf::Texture> texture);

		// Draws the wall on render target, according to draw parameters that were passed.
		void draw(sf::RenderTarget & rt, const WallDrawParameters & params) const;

		// Gets width of the wall.
		float getWidth() const;
		// Returns true if the wall faces the ray. Returning false means this wall is not visible by that ray.
		bool facesRay(const Ray & ray) const;
		// Returns signed distance (positive on the inside of segment) of the point from the wall. Distance is handled correctly even for points, for which 
		// the least distant point of the wall is one of the wall's vertices.
		float distanceFromWall(const sf::Vector2f & point) const;
		// Intersects the wall with a ray. Intersection is returned as out parameter.
		bool intersect(const Ray & ray, WallIntersection & intersection) const;
		// Returns true if line segment intersects the wall.
		bool intersect(const LineSegment & lineSegment_) const;
	};

	class PortalWall : public Wall {
	private:
		portalPtr portal;

	public:

		// Creates a colored wall.
		PortalWall(sf::Vector2f from, sf::Vector2f to, sf::Color color);
		// Creates a wall with color + texture.
		PortalWall(sf::Vector2f from, sf::Vector2f to, sf::Color color, std::shared_ptr<sf::Texture> texture);

		PortalWall(const PortalWall & rs) = default;
		PortalWall(PortalWall && rs) = default;
		PortalWall & operator=(const PortalWall & rs) = default;
		PortalWall & operator=(PortalWall && rs) = default;

		// Returns true if this wall has portal on it.
		bool isPortal() const;
		// Takes object and passes it through portal.
		void stepThrough(ObjectInScene & obj) const;
		// Sets portal for this wall.
		void setPortal(const portalPtr & portal);
	};



	//************************************************************************
	// PREDEFINED WALLS WITH COMMON PORTALS
	//************************************************************************

	// Makes PortalWall that has Door portal on it.
	PortalWall makeDoorWall(sf::Vector2f from, sf::Vector2f to, std::size_t targetSegment);

	// Makes PortalWall that has WallPortal on it. 
	PortalWall makeWallPortalWall(LineSegment wallFrom, LineSegment wallTo, std::size_t targetSegment_);
}

#endif // !PS_WALL_INCLUDED
