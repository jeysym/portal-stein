#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {

	RayCaster::RayCaster() : correctFishbowl(true) {
	}

	void RayCaster::setFishbowlCorrection(bool value)
	{
		correctFishbowl = value;
	}

	void RayCaster::render(sf::RenderTarget & rt, const Scene & scene_)
	{
		// store render dimensions
		renderWidth = rt.getSize().x;
		renderHeight = rt.getSize().y;

		// store pointer to this render target
		renderTarget = &rt;
		// store pointer to the scene
		scene = &scene_;

		for (unsigned int i = 0; i < renderWidth; ++i) {
			auto ray = generateRay(i);

			RenderStripArea area;
			area.column = (float)i;				// currently rendered column of screen
			area.top = 0.0f;					// this initial ray starts at top of the screen ...
			area.bottom = (float)renderHeight;	// ... and ends on the bottom of the screen.

			int initialRecursionDepth = 0;

			renderStip(area, ray, initialRecursionDepth);
		}
	}

	RenderRay RayCaster::generateRay(int i)
	{
		float k = mapIntervals(0.0f, (float)renderWidth - 1.0f, -1.0f, 1.0f, (float)i);
		const Camera & camera = scene->camera;

		sf::Vector2f rayDir = camera.getDirection() + k * camera.viewPlaneDirection;
		RenderRay result( camera.getPosition(), rayDir , camera.getSegmentId() );

		result.correctionFactor = (correctFishbowl) ? dot(result.getDirection(), camera.getDirection()) : 1.0f;	// corrects fishbowl effect
		result.renderFromDistance = 0.1f;	// intial rays render everything from its origin to "infinity"

		return result;
	}

	// Returns the width of the seen portion of the wall, the observer sees with given view angle and given distance from wall.
	float viewWidth(float viewAngle, float objectDistance) {
		return 2.0f * objectDistance * tan(viewAngle / 2.0f);
		// return objectDistance * tan(viewAngle);		// This version needs less mul/div, and for angles close to 0 approximates result well.
	}

	void RayCaster::renderStip(const RenderStripArea & renderStrip, const RenderRay & ray, int recursionDepth)
	{
		// to prevent from cycling when portals create a loop
		if (recursionDepth > recursionLimit)
			return;

		// tries to find the edge in ray segment that ray intersects
		auto & segment = scene->getSegment(ray.getSegmentId());
		for (auto & wall : segment.getWalls()) {

			if (wall.facesRay(ray) == false) {
				// edge is definitely not seen from this ray => skip it
				continue;
			}

			WallIntersection intersection;
			if (wall.intersect(ray, intersection)) {
				//                                    ------x
				//                     |                    |
				//                     x vpWallTop          |
				// ray                 |                    | wall that was hit
				//	x---->	- -	- - - -|                    |
				//                     x vpWallBottom       |
				//                     |              ------x
				//                     |
				//                    view plane
				// |<-  1/correction ->|
				//

				float distance = intersection.rayIntersectionDistance;
				float correctedDistance = distance * ray.correctionFactor;

				float wallTopHeight = segment.segmentFloorHeight + segment.segmentWallHeight;
				float wallBottomHeight = segment.segmentFloorHeight;

				float vpWallTop = distanceToViewPlane(correctedDistance, wallTopHeight);
				float vpWallBottom = distanceToViewPlane(correctedDistance, wallBottomHeight);

				float scrWallTop = viewPlaneToScreen(vpWallTop);
				float scrWallBottom = viewPlaneToScreen(vpWallBottom);
				float scrWallHeight = scrWallBottom - scrWallTop;

				RenderStripArea wallStrip;
				wallStrip.column = renderStrip.column;
				wallStrip.top = scrWallTop;
				wallStrip.bottom = scrWallBottom;

				if (wall.isPortal()) {
					RenderRay rayCopy = ray;										// get a copy of the viewing ray
					wall.stepThrough(rayCopy);										// copy of ray steps through portal
					rayCopy.renderFromDistance = distance;							// this new ray render from the hit wall onwards
					renderStip(wallStrip, rayCopy, recursionDepth + 1);				// edge (segment behind it) is drawn
				}
				else {
					WallDrawParameters drawParams;
					drawParams.scrWallTop = sf::Vector2f(renderStrip.column, scrWallTop );
					drawParams.scrWallBottom = sf::Vector2f(renderStrip.column, scrWallBottom);

					float uvX = intersection.distanceToWallEdge * wall.getWidth();
					drawParams.uvWallTop = sf::Vector2f(uvX, 1 - wallTopHeight);
					drawParams.uvWallBottom = sf::Vector2f(uvX, 1 - wallBottomHeight);

					wall.draw(*renderTarget, drawParams);
				}

				float ceilDH = wallTopHeight - ray.getPosition().z;
				float vpCeilingTop = ceilDH / (ray.renderFromDistance * ray.correctionFactor);
				float scrCeilingTop = viewPlaneToScreen(vpCeilingTop);

				FloorCeilingDrawParameters drawParams;
				drawParams.viewPlaneDistance = 1.0f / ray.correctionFactor;
				drawParams.uvCamera = toVector2(ray.getPosition());
				drawParams.uvDirection = ray.getDirection();

				drawParams.deltaH = ceilDH;
				drawParams.scrTop = sf::Vector2f(renderStrip.column, scrCeilingTop);
				drawParams.scrBottom = sf::Vector2f( renderStrip.column, scrWallTop);
				drawParams.vpTop = vpCeilingTop;
				drawParams.vpBottom = vpWallTop;

				segment.ceiling.draw(*renderTarget, drawParams);

				float floorDH = wallBottomHeight - ray.getPosition().z;
				float vpFloorBottom = floorDH / (ray.renderFromDistance * ray.correctionFactor);
				float scrFloorBottom = viewPlaneToScreen(vpFloorBottom);

				drawParams.scrTop = sf::Vector2f(renderStrip.column, scrWallBottom);
				drawParams.scrBottom = sf::Vector2f(renderStrip.column, scrFloorBottom);
				drawParams.deltaH = floorDH;
				drawParams.vpTop = vpWallBottom;
				drawParams.vpBottom = vpFloorBottom;

				segment.floor.draw(*renderTarget, drawParams);

				return;
			}
		}
	}

	float RayCaster::distanceToViewPlane(float distance, float height)
	{
		return (height - scene->camera.position.z) / distance;
	}

	float RayCaster::viewPlaneToScreen(float x)
	{
		return mapIntervals(scene->camera.viewPlaneHeight, -1.0f * scene->camera.viewPlaneHeight, 0.0f, (float)renderHeight, x);
	}

	RenderRay::RenderRay(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_) : Ray(position_, direction_, segmentId_), correctionFactor(1.0f), renderFromDistance(0.0f)
	{
	}
}