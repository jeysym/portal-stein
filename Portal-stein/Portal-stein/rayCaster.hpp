#pragma once
#ifndef PS_RAYCASTER_INCLUDED
#define PS_RAYCASTER_INCLUDED
#include "Scene.hpp"
#include "SceneObject.hpp"
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {
	class Camera : public SceneObject {
	public:

		Camera(sf::Vector2f origin, sf::Vector2f direction, std::size_t segment, float hFOV, float aspectRatio);

		void rotate(float angle);

		float cameraHeight;
		sf::Vector2f viewPlane;
		float viewPlaneHeight;

	};

	class RayCaster {
	public:
		unsigned int width, height;
		std::shared_ptr<Scene> scene;
		Camera camera;

		RayCaster(unsigned int width_, unsigned int height_, std::shared_ptr<Scene> scene_, Camera camera_);

		void render(sf::RenderTarget & rt);
	};
}

#endif // !PS_RAYCASTER_INCLUDED
