#pragma once
#ifndef PS_RAYCASTER_INCLUDED
#define PS_RAYCASTER_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>
#include "Scene.hpp"
#include "ObjectInScene.hpp"

namespace ps {
	int main();

	class Camera : public ObjectInScene {
	private:
		sf::Vector2f viewPlane;
		float viewPlaneHeight;
		float hFOV, vFOV;

	public:
		Camera(sf::Vector3f origin, sf::Vector2f direction, std::size_t segment);

		// Rotates camera by desired angle (counterclockwise).
		virtual void rotate(float angle);
		// Sets horizontal and vertical field-of-view from horizontal FOV and aspect ratio (width : height).
		void setFOV(float horizontalFOV, float aspectRatio);

		// Camera is part of ray-caster. This lets RayCaster fully access Camera's private members.
		friend class RayCaster;
	};

	class RayWithFishbowlCorrection : public Ray {
	public:
		float correctionFactor;

		RayWithFishbowlCorrection(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_);
	};

	class RayCaster {
	private:
		// limit on recursive renderPart calls
		static constexpr int rescursionLimit = 20;

		// flag indicating if fishbowl effect should be corrected
		bool correctFishbowl;

		// ray-caster has its scene and camera
		scenePtr scene;
		Camera camera;

		// render dimensions
		unsigned int renderWidth;
		unsigned int renderHeight;

		// ray-caster stores pointer to RenderTarget, so it doesn't have to be passed so much while rendering
		sf::RenderTarget * renderTarget;

	public:
		// Constructs a ray-caster. Camera is passed as argument.
		RayCaster(Camera camera_);

		// Sets camera of the ray-caster.
		void setCamera(Camera camera_);
		void moveCamera(sf::Vector3f offset);
		void goForwardCamera(float distance);
		void ascendCamera(float distance);
		void rotateCamera(float angle);

		// Sets the scene that ray-caster renders.
		void setScene(scenePtr scene_);
		// Turns fishbowl correction on/off.
		void setFishbowlCorrection(bool value);
		// Renders the scene from the camera's point of view.
		void render(sf::RenderTarget & rt);

		friend class Game;
	private:

		RayWithFishbowlCorrection generateRay(int i);
		void renderPart(const sf::FloatRect & renderArea, const RayWithFishbowlCorrection & ray, int recursionDepth);

		float distanceToViewPlane(float distance, float height);
		float viewPlaneToScreen(float x);
	};
}

#endif // !PS_RAYCASTER_INCLUDED
