#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {
	RayCaster::RayCaster(unsigned int width_, unsigned int height_, std::shared_ptr<Scene> scene_, Camera camera_) :  width(width_), height(height_), camera(camera_), scene(scene_) { }

	

	void RayCaster::render(sf::RenderTarget & rt)
	{
		for (int i = 0; i < width; ++i) {
			sf::FloatRect renderArea { sf::Vector2f{ (float)i, 0.0f }, sf::Vector2f{ 1.0f, (float)height } }; // stip of the screen that will be drawn

			Ray ray = generateRay(i);

			renderPart(rt, renderArea, ray);

			/*for (auto edge : scene->segments[ray.segmentId].edges) {
				EdgeIntersection i;
				if (edge->intersect(ray, i)) {
					rect.height /= i.rayDist;
					rect.top = height / 2.0f - rect.height / 2.0f;
					i.hitEdge->draw(rt, rect, 1.0f);

					sf::RectangleShape top{ sf::Vector2f{1.0f, 500.0f} };
					top.move(0.0f, -rect.top);
					top.setFillColor(sf::Color::Black);
					rt.draw(top);

					sf::RectangleShape bottom{ sf::Vector2f{1.0f, 500.0f} };
					bottom.move(0.0f, rect.top + rect.height);
					bottom.setFillColor(sf::Color::Black);
					rt.draw(bottom);

					break;
				}
			}*/
		}
	}

	Ray RayCaster::generateRay(int i)
	{
		float k = -1.0f + 1.0f / width + i * 2.0f / width; // just conversion from "i" in [0, width-1] to "k" in (-1, 1)
		sf::Vector2f rayDir = camera.direction + k * camera.viewPlane;
		return Ray{ camera.position, camera.height, rayDir , camera.segmentId };
	}

	void RayCaster::renderPart(sf::RenderTarget & rt, sf::FloatRect renderArea, Ray ray)
	{
		// tries to find the edge in ray segment that ray intersects
		EdgeIntersection intersection;
		auto & segment = scene->segments[ray.segmentId];
		for (auto edge : segment.edges) {
			if (edge->intersect(ray, intersection)) {
				// ray hits this edge
				Edge & hitEdge = *intersection.hitEdge;
				float d = intersection.rayDist;

				float top = projectToScreen(hitEdge.height, d);
				float bottom = projectToScreen(0.0f, d);
				float scrTop = minimum(top, renderArea.top);
				float scrBottom = maximum(bottom, renderArea.top + renderArea.height);

				sf::FloatRect edgeArea{ renderArea.left, top, 1.0f, bottom - top };
				sf::FloatRect ceilingArea{ renderArea.left, scrTop, 1.0f, top - scrTop };
				sf::FloatRect floorArea{ renderArea.left, bottom, 1.0f, scrBottom - bottom };

				if (hitEdge.isPortal()) {
					hitEdge.stepThrough(ray); // ray steps through portal
					renderPart(rt, edgeArea, ray);	// edge (segment behind it) is drawn
				}
				else {
					float edgeWidth = 2.0f * d * tan(camera.hFOV / (2 * width));
					hitEdge.draw(rt, edgeArea, intersection.edgeDist, edgeWidth);
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
		float h = hei - camera.height;
		float H = h * camera.viewPlaneHeight / distance;
		float scrH = (H - camera.viewPlaneHeight) * ((float)height / (- 2.0f * camera.viewPlaneHeight));
		return scrH;
	}

	Camera::Camera(sf::Vector3f origin, sf::Vector2f direction, std::size_t segment, float hFOV_, float aspectRatio) : viewPlane(), SceneHObject(origin, direction, segment)
	{
		hFOV = hFOV_;
		float c = cos(hFOV / 2);
		float viewPlaneNorm = sqrt(1 / (c*c) - 1);	// view plane length (corresponds to horizontal FOV)
		viewPlane = getPerpendicular(direction); // view plane is perpendicular to camera direction
		viewPlane *= viewPlaneNorm;
		viewPlaneHeight = viewPlaneNorm / aspectRatio;	// this is height of view plane (corresponds to vertical FOV (define by aspect ratio))
	}

	void Camera::rotate(float angle)
	{
		rotateVec2(direction, angle);
		rotateVec2(viewPlane, angle);
	}
}