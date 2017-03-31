#pragma once
#ifndef PS_RAYCASTER_INCLUDED
#define PS_RAYCASTER_INCLUDED
#include <memory>
#include <SFML\Graphics.hpp>
#include "Scene.hpp"
#include "ObjectInScene.hpp"

namespace ps {

	//*********************************************************************************
	// RENDERING RAY
	//*********************************************************************************

	// Ray that is used for rendering.
	class RenderRay : public Ray {
	public:
		float correctionFactor;		// fishbowl correction factor
		float renderFromDistance;	// actual distance from which ray starts rendering

		RenderRay(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_);
	};



	//*********************************************************************************
	// RAY CASTER 
	//*********************************************************************************

	// Descries the vertical strip of the screen.
	struct RenderStripArea {
		float column;
		float top;
		float bottom;
	};


	class RayCaster {
	private:
		// limit on recursive renderPart calls
		static constexpr int recursionLimit = 20;

		// flag indicating if fishbowl effect should be corrected
		bool correctFishbowl;

		// ray-caster stores pointer to RenderTarget, so it doesn't have to be passed so much while rendering
		sf::RenderTarget * renderTarget;
		// ray-caster has its scene and camera
		const Scene * scene;

		// render dimensions
		unsigned int renderWidth;
		unsigned int renderHeight;

	public:
		// Constructs a ray-caster.
		RayCaster();

		// Turns fishbowl correction on/off.
		void setFishbowlCorrection(bool value);
		// Renders the scene from the camera's point of view.
		void render(sf::RenderTarget & rt, const Scene & scene);

		friend class Game;
	private:

		RenderRay generateRay(int i);
		void renderStip(const RenderStripArea & renderStrip, const RenderRay & ray, int recursionDepth);

		float distanceToViewPlane(float distance, float height);
		float viewPlaneToScreen(float x);
	};
}

#endif // !PS_RAYCASTER_INCLUDED
