#ifndef FLOOR_AND_CEILING_INCLUDED
#define FLOOR_AND_CEILING_INCLUDED
#include <SFML\Graphics.hpp>
#include <memory>

namespace ps {
	class floor {
		virtual void draw(sf::RenderTarget rt, sf::FloatRect renderRect) = 0;
	};

	class ceiling {
		virtual void draw(sf::RenderTarget rt, sf::FloatRect renderRect) = 0;
	};

	using floor_ptr = std::shared_ptr<floor>;
	using ceiling_ptr = std::shared_ptr<ceiling>;
}

#endif // !FLOOR_AND_CEILING_INCLUDED
