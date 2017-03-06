#pragma once
#ifndef PS_FLOOR_CEILING_INCLUDED
#define PS_FLOOR_CEILING_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {
	class FloorCeiling {
	public:
		virtual void drawPixel(sf::RenderTarget & rt, const sf::FloatRect & renderArea,  float pixelX, float pixelY) = 0;
	};

	class ColoredFloorCeiling : public FloorCeiling {
	private:
		sf::Color color;

	public:
		ColoredFloorCeiling(sf::Color color_);

		void drawPixel(sf::RenderTarget & rt, const sf::FloatRect & renderArea, float pixelX, float pixelY);
	};

	class TexturedFloorCeiling : public FloorCeiling {
	private:
		sf::Texture texture;
		sf::RectangleShape shape;

	public:
		TexturedFloorCeiling(sf::Texture texture_);

		void drawPixel(sf::RenderTarget & rt, const sf::FloatRect & renderArea, float pixelX, float pixelY);
	};

	// Floor and Ceiling are basically the same structure.
	using Floor = FloorCeiling;
	using Ceiling = FloorCeiling;

	using ColoredFloor = ColoredFloorCeiling;
	using ColoredCeiling = ColoredFloorCeiling;

	using TexturedFloor = TexturedFloorCeiling;
	using TexturedCeiling = TexturedFloorCeiling;

	using floorPtr = std::shared_ptr<Floor>;
	using ceilingPtr = std::shared_ptr<Ceiling>;
}

#endif // !PS_FLOOR_CEILING_INCLUDED
