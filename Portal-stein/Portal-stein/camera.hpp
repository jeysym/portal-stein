#pragma once
#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include <SFML\Graphics.hpp>
#include "rayCaster.hpp"

namespace ps {
	struct camera {
		sf::Vector2f position_, direction_;
		float horFOV_, verFOV_;

		camera(sf::Vector2f pos, sf::Vector2f dir, float horFOV, float verFOV) : position_(pos), direction_(dir), horFOV_(horFOV), verFOV_(verFOV) {}


	};
}

#endif // !CAMERA_INCLUDED
