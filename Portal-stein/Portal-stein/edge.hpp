#ifndef EDGE_INCLUDED
#define EDGE_INCLUDED
#include <SFML\Graphics.hpp>
#include "objectInScene.hpp"
#include <memory>

namespace ps {
	class edge {
	public:
		bool isPortal;

		virtual void draw(sf::RenderTarget rt, sf::FloatRect renderRect, sf::IntRect textureRect) = 0;
		virtual void stepThrough(objectInScene obj) = 0;
	};

	using edge_ptr = std::shared_ptr<edge>;
}

#endif // !EDGE_INCLUDED
