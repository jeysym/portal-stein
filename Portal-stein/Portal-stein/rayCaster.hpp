#ifndef RAY_CASTER_INCLUDED
#define RAY_CASTER_INCLUDED
#include "scene.hpp"
#include "objectInScene.hpp"
#include <SFML\Graphics.hpp>

namespace ps {
	class rayCaster {
	public:
		scene scene;
		camera camera;

		void render(sf::RenderTarget rt);
	};
}

#endif // !RAY_CASTER_INCLUDED
