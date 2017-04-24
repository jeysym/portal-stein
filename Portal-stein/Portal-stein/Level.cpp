#include "Level.hpp"

namespace ps {
	Level::Level(std::vector<Segment>&& segments_, ObjectInScene playerPos) : initialScene(playerPos) {
		initialScene.segments = std::move(segments_);
	}

	sf::Texture * Level::addTexture(std::string fileName)
	{
		auto tex = std::make_shared<sf::Texture>();
		tex->loadFromFile(fileName);

		textures.push_back(tex);
		return tex.get();
	}

	Scene Level::makeScene()
	{
		// scene is copied
		return initialScene;
	}
}