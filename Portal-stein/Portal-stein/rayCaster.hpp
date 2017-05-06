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

	/// Ray that is used for rendering.
	class RenderRay : public Ray {
	public:
		float correctionFactor;		///< Fishbowl correction factor.
		float renderFromDistance;	///< Sctual distance from which ray starts rendering.

		RenderRay(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_);
	};



	//*********************************************************************************
	// RAY CASTER 
	//*********************************************************************************

	/// Descries the vertical strip of the screen.
	struct RenderStripArea {
		float column;
		float top;
		float bottom;
	};


	class RayCaster {
	private:
		/// limit on recursive renderPart calls
		static constexpr int recursionLimit = 20;

		bool correctFishbowl;				///< Flag indicating if fishbowl effect should be corrected.
		sf::RenderTarget * renderTarget;	///< Ray-caster stores pointer to RenderTarget, so it doesn't have to be passed so much while rendering.
		const Scene * scene;				///< Ray-caster stores pointer to Scene, so it doesn't have to be passed so much while rendering.

		// render dimensions
		unsigned int renderWidth;
		unsigned int renderHeight;

		RenderRay generateRay(int i);
		void renderStip(const RenderStripArea & renderStrip, const RenderRay & ray, int recursionDepth);

		float distanceToViewPlane(float distance, float height);
		float viewPlaneToScreen(float x);

	public:
		/// Constructs a ray-caster.
		RayCaster();

		/// Turns fishbowl correction on/off.
		void setFishbowlCorrection(bool value);
		/// Renders the scene from the camera's point of view.
		void render(sf::RenderTarget & rt, const Scene & scene);

		friend class Game;
	};
}

#endif // !PS_RAYCASTER_INCLUDED
