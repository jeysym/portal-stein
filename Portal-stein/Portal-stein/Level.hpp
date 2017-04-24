#pragma once
#ifndef PS_LEVEL_INCLUDED
#define PS_LEVEL_INCLUDED
#include <vector>
#include <memory>
#include "SFML\Graphics.hpp"
#include "Scene.hpp"

namespace ps {

	class Level {
	private:
		std::vector<std::shared_ptr<sf::Texture>> textures;
		Scene initialScene;

	public:
		Level(std::vector<Segment> && segments_, ObjectInScene playerPos);

		sf::Texture * addTexture(std::string fileName);
		Scene makeScene();
	};

}

#endif // !PS_LEVEL_INCLUDED
