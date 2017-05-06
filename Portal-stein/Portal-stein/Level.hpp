#pragma once
#ifndef PS_LEVEL_INCLUDED
#define PS_LEVEL_INCLUDED
#include <vector>
#include <memory>
#include "SFML\Graphics.hpp"
#include "Scene.hpp"

namespace ps {

	// Represents level for the game. That means all the segments (+ textures).
	class Level {
	private:
		std::vector<std::shared_ptr<sf::Texture>> textures;
		Scene initialScene;

	public:
		Level(std::vector<Segment> && segments_, ObjectInScene playerPos);

		// Loads texture from file.
		sf::Texture * addTexture(std::string fileName);
		// Makes scene that corresponds to the initial state of this level. (Can be called multiple times)
		Scene makeScene();
	};

}

#endif // !PS_LEVEL_INCLUDED
