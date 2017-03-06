#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {

	RayCaster::RayCaster(Camera camera_) : camera(camera_), correctFishbowl(true) { }

	void RayCaster::setScene(scenePtr scene_) {
		scene = scene_;
	}

	void RayCaster::setFishbowlCorrection(bool value)
	{
		correctFishbowl = value;
	}

	void RayCaster::setCamera(Camera camera_)
	{
		camera = camera_;
	}

	void RayCaster::moveCamera(sf::Vector3f offset)
	{
		auto oldCameraPosition = camera.getPosition();
		camera.move(offset);
		auto newCameraPosition = camera.getPosition();
		LineSegment cameraLS{ toVector2(oldCameraPosition), toVector2(newCameraPosition) };

		// check whether the camera didn't pass through wall
		auto & segment = scene->getSegment(camera.getSegmentId());
		for (auto & wall : segment.edges) {
			if (wall->intersect(cameraLS)) {
				// camera would move through the wall
				if (wall->isPortal()) {
					// camera steps through the portal
					wall->stepThrough(camera);
				}
				else {
					// camera wants to pass solid wall => put it back
					camera.position = oldCameraPosition;
				}
			}
		}
	}

	void RayCaster::goForwardCamera(float distance)
	{
		moveCamera(distance * toVector3(camera.getDirection()));
	}

	void RayCaster::ascendCamera(float distance)
	{
		camera.ascend(distance);
	}

	void RayCaster::rotateCamera(float angle)
	{
		camera.rotate(angle);
	}

	void RayCaster::render(sf::RenderTarget & rt)
	{
		// store render dimensions
		renderWidth = rt.getSize().x;
		renderHeight = rt.getSize().y;

		// store pointer to this render target
		this->renderTarget = &rt;

		for (unsigned int i = 0; i < renderWidth; ++i) {
			sf::FloatRect renderArea { sf::Vector2f{ (float)i, 0.0f }, sf::Vector2f{ 1.0f, (float)renderHeight } }; // stip of the screen that will be drawn

			auto ray = generateRay(i);

			renderPart(renderArea, ray, 0);
		}
	}

	RayWithFishbowlCorrection RayCaster::generateRay(int i)
	{
		float k = -1.0f + 1.0f / renderWidth + i * 2.0f / renderWidth;								// just conversion from "i" in [0, width-1] to "k" in (-1, 1)
		sf::Vector2f rayDir = camera.getDirection() + k * camera.viewPlane;
		RayWithFishbowlCorrection result{ camera.getPosition(), rayDir , camera.getSegmentId() };
		result.correctionFactor = dot(result.getDirection(), camera.getDirection());
		return result;
	}

	// Returns the width of the seen portion of the wall, the observer sees with given view angle and given distance from wall.
	float viewWidth(float viewAngle, float objectDistance) {
		return 2.0f * objectDistance * tan(viewAngle / 2.0f);
		// return objectDistance * tan(viewAngle);		// This version needs less mul/div, and for angles close to 0 approximates result well.
	}

	void fillWithBlack(sf::RenderTarget & rt, const sf::FloatRect & renderArea) {
		sf::RectangleShape blackRectangle;
		blackRectangle.setPosition(renderArea.left, renderArea.top);
		blackRectangle.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		blackRectangle.setFillColor(sf::Color::Black);

		rt.draw(blackRectangle);
	}

	void RayCaster::renderPart(const sf::FloatRect & renderArea, const RayWithFishbowlCorrection & ray, int recursionDepth)
	{
		// to prevent from cycling when portals create a loop
		if (rescursionLimit <= recursionDepth)
			return;

		// tries to find the edge in ray segment that ray intersects
		WallIntersection intersection;
		auto & segment = scene->getSegment(ray.getSegmentId());
		for (auto edge : segment.edges) {

			if (edge->facesRay(ray) == false) {
				// edge is definitely not seen from this ray => skip it
				continue;
			}

			if (edge->intersect(ray, intersection)) {
				// ray hits this edge
				Wall & hitEdge = *intersection.wallThatWasHit;
				float distance = intersection.rayIntersectionDistance;
				float correctedDistance = distance * ray.correctionFactor;

				float wallTopHeight = segment.segmentFloorHeight + segment.segmentWallHeight;
				float wallBottomHeight = segment.segmentFloorHeight;

				float vpWallTop = distanceToViewPlane(correctedDistance, wallTopHeight);
				float vpWallBottom = distanceToViewPlane(correctedDistance, wallBottomHeight);

				float scrWallTop = viewPlaneToScreen(vpWallTop);
				float scrWallBottom = viewPlaneToScreen(vpWallBottom);
				float scrWallHeight = scrWallBottom - scrWallTop;

				sf::FloatRect wallScreenArea{ renderArea.left, scrWallTop, 1.0f, scrWallHeight };

				if (hitEdge.isPortal()) {
					RayWithFishbowlCorrection rayCopy = ray;						// get a copy of the viewing ray
					hitEdge.stepThrough(rayCopy);									// copy of ray steps through portal
					renderPart(wallScreenArea, rayCopy, recursionDepth + 1);		// edge (segment behind it) is drawn
				}
				else {
					sf::FloatRect wallArea;
					wallArea.left = intersection.distanceToWallEdge * hitEdge.getWidth();
					wallArea.top = 1 - wallTopHeight;
					wallArea.width = viewWidth(camera.hFOV / renderWidth, distance);
					wallArea.height = segment.segmentWallHeight;
					hitEdge.draw(*renderTarget, wallScreenArea, wallArea);
				}

				floorPtr floor = segment.floor;
				ceilingPtr ceiling = segment.ceiling;

				float hT = wallTopHeight - ray.getPosition().z;
				float ceilingPixelVPheight = vpWallTop;
				sf::FloatRect ceilingPixel{ renderArea.left, scrWallTop - 1.0f, 1.0f, 1.0f };
				while (ceilingPixel.top > renderArea.top) {

					float p = hT / (ceilingPixelVPheight * ray.correctionFactor);
					sf::Vector2f ceilingPoint = toVector2(ray.getPosition()) + p * ray.getDirection();

					ceiling->drawPixel(*renderTarget, ceilingPixel, ceilingPoint.x, ceilingPoint.y);

					ceilingPixelVPheight += 2.0f * (camera.viewPlaneHeight / renderHeight);
					ceilingPixel.top -= 1.0f;
				}

				float hB = ray.getPosition().z - wallBottomHeight;
				float floorPixelVPHeight = -1.0f * vpWallBottom;
				sf::FloatRect floorPixel{ renderArea.left, scrWallBottom, 1.0f, 1.0f };
				while (floorPixel.top < (renderArea.top + renderArea.height)) {

					float p = hB / (floorPixelVPHeight * ray.correctionFactor);
					sf::Vector2f floorPoint = toVector2(ray.getPosition()) + p * ray.getDirection();

					floor->drawPixel(*renderTarget, floorPixel, floorPoint.x, floorPoint.y);

					floorPixelVPHeight += 2.0f * (camera.viewPlaneHeight / renderHeight);
					floorPixel.top += 1.0f;
				}

				return;
			}
		}

		// Ray hit no edge => color the area black
		fillWithBlack(*renderTarget, renderArea);
	}

	float RayCaster::distanceToViewPlane(float distance, float height)
	{
		return (height - camera.position.z) / distance;
	}

	float RayCaster::viewPlaneToScreen(float x)
	{
		return mapIntervals(camera.viewPlaneHeight, -1.0f * camera.viewPlaneHeight, 0.0f, (float)renderHeight, x);
	}

	//float RayCaster::projectToScreen(float hei, float distance)
	//{
	//	float h = hei - camera.getPosition().z;
	//	float H = h * camera.viewPlaneHeight / distance;
	//	float scrH = (H - camera.viewPlaneHeight) * ((float)renderHeight / (- 2.0f * camera.viewPlaneHeight));
	//	return scrH;
	//}

	Camera::Camera(sf::Vector3f origin, sf::Vector2f direction, std::size_t segment) : viewPlane(), ObjectInScene(origin, direction, segment)
	{
		float defaultHFOV = 0.4f * PI<float>;		// default horizontal fov is approx. 72 degrees
		float defaultAspectRatio = 16.0f / 9.0f;	// expect widescreen monitor by default
		setFOV(defaultHFOV, defaultAspectRatio);
	}

	void Camera::rotate(float angle)
	{
		ps::rotate(direction, angle);
		ps::rotate(viewPlane, angle);
	}

	void Camera::setFOV(float horizontalFOV, float aspectRatio)
	{
		if (0.0f < horizontalFOV && 0.0f < aspectRatio) {
			hFOV = horizontalFOV;
			float viewPlaneNorm = tan(horizontalFOV / 2.0f);	// view plane length (corresponds to horizontal FOV)

			// sets viewPlane to be perpendicuar to direction
			viewPlane.x = direction.y;
			viewPlane.y = -1.0f * direction.x;

			viewPlane *= viewPlaneNorm;
			viewPlaneHeight = viewPlaneNorm / aspectRatio;		// this is height of view plane (corresponds to vertical FOV (define by aspect ratio))
		}
	}
	RayWithFishbowlCorrection::RayWithFishbowlCorrection(sf::Vector3f position_, sf::Vector2f direction_, std::size_t segmentId_) : Ray(position_, direction_, segmentId_), correctionFactor(1.0f)
	{
	}
}