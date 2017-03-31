#include "FloorCeiling.hpp"
#include <exception>
#include "Math.hpp"

namespace ps {

	sf::Shader FloorCeiling::shader;

	void FloorCeiling::compileShaders()
	{
		std::string vertexShaderCode = R"raw(
		void main() {
			// transform the vertex position 
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
			// transform the texture coordinates
			gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
			// forward the color
			gl_FrontColor = gl_Color;
		}
		)raw";

		std::string fragmentShaderCode = R"raw(
		uniform float hD;
		uniform float vpDistance;
		uniform vec2 from;
		uniform vec2 dir;
		uniform sampler2D myTexture;
	
		void main() {
			float sigma = gl_TexCoord[0].x / gl_TexCoord[0].y;
			float d = (hD * vpDistance) / sigma;
			vec2 point = from + d * dir;
			vec4 pixel = texture(myTexture, point);
		
			gl_FragColor = gl_Color * pixel;
		};
		)raw";

		bool compileSuccess = FloorCeiling::shader.loadFromMemory(vertexShaderCode, fragmentShaderCode);
		if (compileSuccess == false)
			throw std::runtime_error("Shader for floor/ceiling was not successfuly compiled!");
		
		shader.setUniform("myTexture", sf::Shader::CurrentTexture);
	}

	void FloorCeiling::draw(sf::RenderTarget & rt, const FloorCeilingDrawParameters & params) const {
		shader.setUniform("hD", params.deltaH);
		shader.setUniform("from", params.uvCamera);
		shader.setUniform("dir", params.uvDirection);

		shader.setUniform("vpDistance", params.viewPlaneDistance);

		sf::VertexArray arr(sf::Lines, 2);
		arr[0] = sf::Vertex(params.scrTop, color, sf::Vector2f(params.vpTop, 1.0f));
		arr[1] = sf::Vertex(params.scrBottom, color, sf::Vector2f(params.vpBottom, 1.0f));
	
		sf::RenderStates states;
		states.shader = &shader;
		states.texture = texture;
		rt.draw(arr, states);
	}

	FloorCeiling::FloorCeiling(const sf::Color & color_) : FloorCeiling(color, nullptr)	{
	}

	FloorCeiling::FloorCeiling(const sf::Color & color_, sf::Texture * texture_) : color(color_), texture(texture_) {
		if (texture != nullptr)
			texture->setRepeated(true);
	}

}