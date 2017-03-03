#pragma once
#ifndef PS_RAYCASTER_INCLUDED
#define PS_RAYCASTER_INCLUDED
#include "Scene.hpp"
#include "ObjectInScene.hpp"
#include <memory>
#include <SFML\Graphics.hpp>

namespace ps {
	class Camera : public ObjectInScene {
	public:

		Camera(sf::Vector3f origin, sf::Vector2f direction, std::size_t segment, float hFOV, float aspectRatio);

		virtual void rotate(float angle);

		sf::Vector2f viewPlane;
		float viewPlaneHeight;
		float hFOV, vFOV;
	};

	class RayCaster {
	private:
		scenePtr scene;
		Camera camera;
		unsigned int renderWidth;
		unsigned int renderHeight;

	public:
		RayCaster(Camera camera_);

		void setScene(scenePtr scene_);
		Camera& getCamera();
		void render(sf::RenderTarget & rt);

	private:
		Ray generateRay(int i);
		void renderPart(sf::RenderTarget & rt, sf::FloatRect renderArea, Ray ray);
		float projectToScreen(float height, float distance);
	};
}

#endif // !PS_RAYCASTER_INCLUDED
