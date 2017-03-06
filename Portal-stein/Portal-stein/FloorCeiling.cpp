#include "FloorCeiling.hpp"

namespace ps {




	ColoredFloorCeiling::ColoredFloorCeiling(sf::Color color_) : color(color_)
	{
	}

	void ColoredFloorCeiling::drawPixel(sf::RenderTarget & rt, const sf::FloatRect & renderArea, float pixelX, float pixelY)
	{
		sf::RectangleShape rs;
		rs.setPosition(renderArea.left, renderArea.top);
		rs.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		rs.setFillColor(color);
		rt.draw(rs);
	}

	TexturedFloorCeiling::TexturedFloorCeiling(sf::Texture texture_) : texture(texture_), shape(sf::Vector2f{ 1.0f, 1.0f })
	{
		texture.setRepeated(true);
		shape.setTexture(&texture);
	}

	void TexturedFloorCeiling::drawPixel(sf::RenderTarget & rt, const sf::FloatRect & renderArea, float pixelX, float pixelY)
	{
		shape.setPosition(renderArea.left, renderArea.top);
		shape.setSize(sf::Vector2f{ renderArea.width, renderArea.height });

		int texLeft = (int)(pixelX * texture.getSize().x);
		int texTop = (int)(pixelY * texture.getSize().y);
		shape.setTextureRect(sf::IntRect{texLeft, texTop, 1, 1});

		rt.draw(shape);
	}

}