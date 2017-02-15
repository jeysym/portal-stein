#include "FloorCeiling.hpp"

namespace ps {
	ColoredFloor::ColoredFloor(sf::Color color_) : color(color_) { }

	void ColoredFloor::draw(sf::RenderTarget & rt, sf::FloatRect drawArea, sf::Vector2f edgeBottom, sf::Vector2f screenBottom)
	{
		sf::RectangleShape rect;
		rect.setPosition(drawArea.left, drawArea.top);
		rect.setSize(sf::Vector2f{ drawArea.width, drawArea.height });
		rect.setFillColor(color);
		rt.draw(rect);
	}

	ColoredCeiling::ColoredCeiling(sf::Color color_) : color(color_) {	}

	void ColoredCeiling::draw(sf::RenderTarget & rt, sf::FloatRect drawArea, sf::Vector2f edgeTop, sf::Vector2f screenTop)
	{
		sf::RectangleShape rect;
		rect.setPosition(drawArea.left, drawArea.top);
		rect.setSize(sf::Vector2f{ drawArea.width, drawArea.height });
		rect.setFillColor(color);
		rt.draw(rect);
	}

}