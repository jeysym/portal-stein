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
		room0.edges.push_back(std::make_unique<ColoredWall>(a, b, sf::Color::Blue));
		room0.edges.push_back(std::make_unique<ColoredWall>(b, c, sf::Color::Red));
		room0.edges.push_back(std::make_unique<WallPortalWall>(LineSegment{ c, d }, LineSegment{ g, f }, 1));

		Segment room1{ floor, ceiling };
		room1.edges.push_back(std::make_unique<TexturedWall>(e, f, metalTex));
		//room1.edges.push_back(std::make_unique<TexturedWall>(f, g, metalTex));
		room1.edges.push_back(std::make_unique<WallPortalWall>(LineSegment{ f, g }, LineSegment{ d, c }, 0));
		room1.edges.push_back(std::make_unique<ColoredWall>(g, h, sf::Color::Yellow));
		room1.edges.push_back(std::make_unique<ColoredWall>(h, e, sf::Color::Magenta));

		result->addSegment(room0);
		result->addSegment(room1);

		return result;
	}

	int main() {
		unsigned int wWidth = 800;
		unsigned int wHeight = 600;

		float walkSpeed = 3.8f / 1000.0f;
		float rotateSpeed = 2.5f / 1000.0f;
		float ascendSpeed = 1.2f / 1000.0f;

		Camera camera{ sf::Vector3f{4.0f, 1.0f, 0.1f}, sf::Vector2f{ 1.0f, -1.0f}, 0 };
		auto scene = makeTestScene();

		RayCaster caster{ camera };
		caster.setScene(scene);

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
						caster.goForwardCamera(walkSpeed * msElapsed);
						break;
					case sf::Keyboard::S:
						caster.goForwardCamera(-walkSpeed * msElapsed);
						break;
					case sf::Keyboard::A:
						caster.rotateCamera(rotateSpeed * msElapsed);
						break;
					case sf::Keyboard::D:
						caster.rotateCamera(-rotateSpeed * msElapsed);
						break;
					case sf::Keyboard::Q:
						caster.ascendCamera(ascendSpeed * msElapsed);
						break;
					case sf::Keyboard::E:
						caster.ascendCamera(-ascendSpeed * msElapsed);
						break;
					}
				}
			}

			window.clear(sf::Color::Black);

			caster.render(window);

			auto position = caster.camera.getPosition();
			auto direction = caster.camera.getDirection();
			auto segmentId = caster.camera.getSegmentId();
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
