#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {
	RayCaster::RayCaster(unsigned int width_, unsigned int height_, std::shared_ptr<Scene> scene_, Camera camera_) :  width(width_), height(height_), camera(camera_), scene(scene_) { }

	void RayCaster::render(sf::RenderTarget & rt)
	{
		for (int i = 0; i < width; ++i) {
			sf::FloatRect rect{ sf::Vector2f{ (float)i, 0.0f }, sf::Vector2f{ 1.0f, (float)height } };

			float k = -1.0f + 1.0f / width + i * 2.0f / width;
			sf::Vector2f rayDir = camera.direction + k * camera.viewPlane;
			Ray ray{ camera.position, rayDir , camera.segmentId };

			for (auto edge : scene->segments[ray.segmentId].edges) {
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
			}
		}
	}

	Camera::Camera(sf::Vector2f origin, sf::Vector2f direction, std::size_t segment, float hFOV, float aspectRatio) : viewPlane(), SceneObject(origin, direction, segment)
	{
		float c = cos(hFOV / 2);
		float viewPlaneNorm = sqrt(1 / (c*c) - 1);
		viewPlane = getPerpendicular(direction);
		viewPlane *= viewPlaneNorm;
		viewPlaneHeight = viewPlaneNorm / aspectRatio;
	}

	void Camera::rotate(float angle)
	{
		rotateVec2(direction, angle);
		rotateVec2(viewPlane, angle);
	}
}