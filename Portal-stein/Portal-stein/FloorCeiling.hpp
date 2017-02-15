#pragma once
#ifndef PS_FLOOR_CEILING_INCLUDED
#define PS_FLOOR_CEILING_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {
	class Floor {
	public:
		virtual void draw(sf::RenderTarget & rt, sf::FloatRect drawArea, sf::Vector2f edgeBottom, sf::Vector2f screenBottom) = 0;
	};

	class Ceiling {
	public:
		virtual void draw(sf::RenderTarget & rt, sf::FloatRect drawArea, sf::Vector2f edgeTop, sf::Vector2f screenTop) = 0;
	};

	class ColoredFloor : public Floor {
	public:
		ColoredFloor(sf::Color color_);

		void draw(sf::RenderTarget & rt, sf::FloatRect drawArea, sf::Vector2f edgeBottom, sf::Vector2f screenBottom);

	private:
		sf::Color color;
	};

	class ColoredCeiling : public Ceiling {
	public:
		ColoredCeiling(sf::Color color_);

		void draw(sf::RenderTarget & rt, sf::FloatRect drawArea, sf::Vector2f edgeTop, sf::Vector2f screenTop);

	private:
		sf::Color color;
	};

	using floorPtr = std::shared_ptr<Floor>;
	using ceilingPtr = std::shared_ptr<Ceiling>;
}

#endif // !PS_FLOOR_CEILING_INCLUDED
