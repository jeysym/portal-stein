#include "FloorCeiling.hpp"

namespace ps {

	ColoredFloorCeiling::ColoredFloorCeiling(sf::Color color_) : color(color_) {
	}

	void ColoredFloorCeiling::draw(sf::RenderTarget & rt, sf::VertexArray & vertexArray)
	{
		for (int i = 0; i < vertexArray.getVertexCount(); ++i) {
			vertexArray[i].color = color;
		}

		rt.draw(vertexArray);
	}

	TexturedFloorCeiling::TexturedFloorCeiling(sf::Texture texture_) : texture(texture_)
	{
		texture.setRepeated(true);
	}

	void TexturedFloorCeiling::draw(sf::RenderTarget & rt, sf::VertexArray & vertexArray)
	{
		for (int i = 0; i < vertexArray.getVertexCount(); ++i) {
			vertexArray[i].texCoords.x *= texture.getSize().x;
			vertexArray[i].texCoords.y *= texture.getSize().y;
		}

		rt.draw(vertexArray, &texture);
	}



}