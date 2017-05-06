#pragma once
#ifndef PS_FLOOR_CEILING_INCLUDED
#define PS_FLOOR_CEILING_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {

	// Parameters that are passed, when floor (or ceiling) is drawn.
	struct FloorCeilingDrawParameters {
		sf::Vector2f scrTop;
		sf::Vector2f scrBottom;

		sf::Vector2f uvCamera;
		sf::Vector2f uvDirection;

		float deltaH;
		float viewPlaneDistance;
		float vpTop;
		float vpBottom;
	};

	//**************************************************************************
	// FLOOR AND CEILING CLASS
	//**************************************************************************

	// Class representing floor (ceiling) that has color and texture.
	class FloorCeiling {
	private:
		static sf::Shader shader;	// GLSL shader for displaying floors and ceilings.

		sf::Color color;
		std::shared_ptr<sf::Texture> texture;

	public:
		static void compileShaders();

		// Draws this floor / ceiling according to draw parameters.
		void draw(sf::RenderTarget & rt, const FloorCeilingDrawParameters & params) const;

		// Creates colored floor/ceiling.
		FloorCeiling(const sf::Color & color_);
		// Creates floor/ceiling with color + texture.
		FloorCeiling(const sf::Color & color_, std::shared_ptr<sf::Texture> texture_);
	};



	//**************************************************************************
	// ADDITIONAL TYPE DEFINITIONS
	//**************************************************************************

	// Floor and Ceiling are basically the same structure. Theese structures are just for type safety.
	using Floor = FloorCeiling;
	using Ceiling = FloorCeiling;
}

#endif // !PS_FLOOR_CEILING_INCLUDED
