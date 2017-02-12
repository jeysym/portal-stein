#ifndef OBJECT_IN_SCENE_INCLUDED
#define OBJECT_IN_SCENE_INCLUDED
#include <SFML\Graphics.hpp>

namespace ps {
	class objectInScene {
	public:
		sf::Vector2f position;
		sf::Vector2f direction;
		std::size_t segmentId;
	};

	class ray : public objectInScene {

	};

	class camera : public objectInScene {
	public:
		float focalDistance;

		ray getRay(float i);
	};
}

#endif // !OBJECT_IN_SCENE_INCLUDED
