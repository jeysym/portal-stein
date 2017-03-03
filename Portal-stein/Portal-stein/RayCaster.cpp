#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {
	RayCaster::RayCaster(Camera camera_) : camera(camera_) { }

	void RayCaster::setScene(scenePtr scene_) {
		scene = scene_;
	}

	Camera & RayCaster::getCamera()
	{
		return camera;
	}

	void RayCaster::render(sf::RenderTarget & rt)
	{
		renderWidth = rt.getSize().x;
		renderHeight = rt.getSize().y;

		for (unsigned int i = 0; i < renderWidth; ++i) {
			sf::FloatRect renderArea { sf::Vector2f{ (float)i, 0.0f }, sf::Vector2f{ 1.0f, (float)renderHeight } }; // stip of the screen that will be drawn

			Ray ray = generateRay(i);

			renderPart(rt, renderArea, ray);
		}
	}

	Ray RayCaster::generateRay(int i)
	{
		float k = -1.0f + 1.0f / renderWidth + i * 2.0f / renderWidth; // just conversion from "i" in [0, width-1] to "k" in (-1, 1)
		sf::Vector2f rayDir = camera.getDirection() + k * camera.viewPlane;
		return Ray{ camera.getPosition(), rayDir , camera.getSegmentId() };
	}

	void RayCaster::renderPart(sf::RenderTarget & rt, sf::FloatRect renderArea, Ray ray)
	{
		// tries to find the edge in ray segment that ray intersects
		WallIntersection intersection;
		auto & segment = scene->getSegment(ray.getSegmentId());
		for (auto edge : segment.edges) {
			if (edge->intersect(ray, intersection)) {
				// ray hits this edge
				Wall & hitEdge = *intersection.wallThatWasHit;
				float distance = intersection.rayIntersectionDistance;//dot(intersection.rayIntersectionDistance * ray.direction, camera.direction); // fishbowl effect will be removed

				float top = projectToScreen(segment.segmentFloorHeight + segment.segmentWallHeight, distance);
				float bottom = projectToScreen(segment.segmentFloorHeight, distance);
				float scrTop = getMin(top, renderArea.top);
				float scrBottom = getMax(bottom, renderArea.top + renderArea.height);

				sf::FloatRect edgeArea{ renderArea.left, top, 1.0f, bottom - top };
				sf::FloatRect ceilingArea{ renderArea.left, scrTop, 1.0f, top - scrTop };
				sf::FloatRect floorArea{ renderArea.left, bottom, 1.0f, scrBottom - bottom };

				if (hitEdge.isPortal()) {
					//auto oldCamera = camera;
					hitEdge.stepThrough(ray); // ray steps through portal
					//hitEdge.stepThrough(&camera);
					renderPart(rt, edgeArea, ray);	// edge (segment behind it) is drawn
					//camera = oldCamera;
				}
				else {
					float edgeWidth = 1.0f;// 2.0f * distance * tan(camera.hFOV / (2 * renderWidth));
					hitEdge.draw(rt, edgeArea, intersection.distanceToWallEdge, edgeWidth);
				}

				segment.floor->draw(rt, floorArea, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 0.0f, 0.0f }); // TODO: make floor & edge draw correctly (for textured floor)
				segment.ceiling->draw(rt, ceilingArea, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 0.0f, 0.0f });

				return;
			}
		}

		// Ray hit no edge => color the area black
		sf::RectangleShape drawRect;
		drawRect.setPosition(renderArea.left, renderArea.top);
		drawRect.setSize(sf::Vector2f{ renderArea.width, renderArea.height });
		drawRect.setFillColor(sf::Color::Black);
		rt.draw(drawRect);
	}

	float RayCaster::projectToScreen(float hei, float distance)
	{
		float h = hei - camera.getPosition().z;
		float H = h * camera.viewPlaneHeight / distance;
		float scrH = (H - camera.viewPlaneHeight) * ((float)renderHeight / (- 2.0f * camera.viewPlaneHeight));
		return scrH;
	}

	Camera::Camera(sf::Vector3f origin, sf::Vector2f direction, std::size_t segment, float hFOV_, float aspectRatio) : viewPlane(), ObjectInScene(origin, direction, segment)
	{
		hFOV = hFOV_;
		float c = cos(hFOV / 2);
		float viewPlaneNorm = sqrt(1 / (c*c) - 1);	// view plane length (corresponds to horizontal FOV)
		
		viewPlane.x = direction.y;
		viewPlane.y = -direction.x;
		viewPlane *= viewPlaneNorm;

		viewPlaneHeight = viewPlaneNorm / aspectRatio;	// this is height of view plane (corresponds to vertical FOV (define by aspect ratio))
	}

	void Camera::rotate(float angle)
	{
		ps::rotate(direction, angle);
		ps::rotate(viewPlane, angle);
	}
}