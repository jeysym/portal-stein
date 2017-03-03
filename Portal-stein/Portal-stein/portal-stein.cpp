#include <SFML/Graphics.hpp>
#include "RayCaster.hpp"
#include "Math.hpp"
#include "FloorCeiling.hpp"

namespace ps {

	std::shared_ptr<Scene> makeTestScene() {
		auto result = std::make_shared<Scene>();

		sf::Texture wallTex;
		wallTex.loadFromFile("H:\\MyData\\Textures\\wall.bmp");
		sf::Texture metalTex;
		metalTex.loadFromFile("H:\\MyData\\Textures\\metal.bmp");
		sf::Texture floorTex;
		floorTex.loadFromFile("H:\\MyData\\Textures\\floor.bmp");
		
		floorPtr floor = std::make_shared<ColoredFloor>( sf::Color::Green );
		ceilingPtr ceiling = std::make_shared<ColoredCeiling>( sf::Color::Cyan );

		sf::Vector2f a{ 0.0f, 0.0f };
		sf::Vector2f b{ 0.0f, 5.0f };
		sf::Vector2f c{ 5.0f, 5.0f };
		sf::Vector2f d{ 5.0f, 0.0f };

		sf::Vector2f offset{ 10.0, 0.0 };
		sf::Vector2f e = a + offset;
		sf::Vector2f f = b + offset;
		sf::Vector2f g = c + offset;
		sf::Vector2f h = d + offset;

		Segment room0{ floor, ceiling };
		room0.edges.push_back(std::make_unique<TexturedWall>(d, a, wallTex));
		room0.edges.push_back(std::make_unique<TexturedWall>(a, b, wallTex));
		room0.edges.push_back(std::make_unique<TexturedWall>(b, c, wallTex));
		room0.edges.push_back(std::make_unique<WallPortalWall>(LineSegment{ c, d }, LineSegment{ g, f }, 1));

		Segment room1{ floor, ceiling };
		room1.edges.push_back(std::make_unique<TexturedWall>(e, f, metalTex));
		room1.edges.push_back(std::make_unique<TexturedWall>(g, h, metalTex));
		room1.edges.push_back(std::make_unique<TexturedWall>(h, e, metalTex));
		room1.edges.push_back(std::make_unique<DoorWall>(e, f, 0));

		result->addSegment(room0);
		result->addSegment(room1);

		return result;
	}

	int main() {
		unsigned int wWidth = 800;
		unsigned int wHeight = 600;

		float walkSpeed = 1.8f / 1000.0f;
		float rotateSpeed = 1.5f / 1000.0f;
		float ascendSpeed = 1.2f / 1000.0f;
		float hFOV = PI<float> * 0.36f;

		

		Camera camera{ sf::Vector3f{4.0f, 1.0f, 0.1f}, sf::Vector2f{ 1.0f, -1.0f}, 0, hFOV, (float)wWidth/(float)wHeight };
		auto scene = makeTestScene();

		RayCaster caster{ camera };
		caster.setScene(scene);

		Camera & casterCamera = caster.getCamera();

		sf::RenderWindow window{ sf::VideoMode{wWidth, wHeight}, "Portal-stein" };
		window.setVerticalSyncEnabled(true);

		sf::Font font;
		font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
		sf::Text info{ "NA", font };
		info.setFillColor(sf::Color::Black);

		// get the clock object
		sf::Clock clock;

		while (window.isOpen())
		{
			// measure the time elapsed from the last draw
			float msElapsed = (float)clock.getElapsedTime().asMilliseconds();
			clock.restart();

			sf::Event event;
			while (window.pollEvent(event))
			{
				// dispatch all the events in the queue

				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::W:
						casterCamera.goForward(walkSpeed * msElapsed);
						break;
					case sf::Keyboard::S:
						casterCamera.goForward(-walkSpeed * msElapsed);
						break;
					case sf::Keyboard::A:
						casterCamera.rotate(rotateSpeed * msElapsed);
						break;
					case sf::Keyboard::D:
						casterCamera.rotate(-rotateSpeed * msElapsed);
						break;
					case sf::Keyboard::Q:
						casterCamera.ascend(ascendSpeed * msElapsed);
						break;
					case sf::Keyboard::E:
						casterCamera.ascend(-ascendSpeed * msElapsed);
						break;
					}
				}
			}

			window.clear(sf::Color::Black);

			caster.render(window);

			auto position = casterCamera.getPosition();
			auto direction = casterCamera.getDirection();
			auto segmentId = casterCamera.getSegmentId();
			info.setString(
				"pos = [" + std::to_string(position.x) + "," + std::to_string(position.y) + "]\n" + 
				"dir = [" + std::to_string(direction.x) + "," + std::to_string(direction.y) + "]\n" + 
				"segment = " + std::to_string(segmentId) + "\n" + 
				"fps = " + std::to_string((int)(1000.0f / msElapsed))
			);

			window.draw(info);

			window.display();
		}

		return 0;
	}
}

int main() {
	return ps::main();
}
