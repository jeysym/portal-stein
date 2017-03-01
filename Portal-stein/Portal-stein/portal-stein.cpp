#include <SFML/Graphics.hpp>
#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {

	std::shared_ptr<Scene> makeTestScene() {
		sf::Texture wallTex;
		wallTex.loadFromFile("H:\\MyData\\Textures\\wall.bmp");
		sf::Texture metalTex;
		metalTex.loadFromFile("H:\\MyData\\Textures\\metal.bmp");

		sf::Vector2f lt{ -2.0f, 2.0f };
		sf::Vector2f lb{ -2.0f, -2.0f };
		sf::Vector2f rt{ 2.0f, 2.0f };
		sf::Vector2f rb{ 2.0f, -2.0f };

		sf::Vector2f other{ 5.0f, 0.0f };

		auto scene = std::make_shared<Scene>();
		floorPtr floor = std::make_shared<ColoredFloor>(sf::Color::Green);
		floorPtr greyFloor = std::make_shared<ColoredFloor>(sf::Color{ 181, 179, 168 });
		ceilingPtr ceiling = std::make_shared<ColoredCeiling>(sf::Color::Cyan);

		Segment segment0{ floor, ceiling };
		segment0.edges.push_back(std::make_shared<ColoredWall>(lt, rt, sf::Color::Blue));
		segment0.edges.push_back(std::make_shared<DoorWall>(rt, rb, 1));
		segment0.edges.push_back(std::make_shared<TexturedWall>(rb, lb, metalTex));
		segment0.edges.push_back(std::make_shared<ColoredWall>(lb, lt, sf::Color::Yellow));

		Segment segment1{ greyFloor, ceiling };
		segment1.edges.push_back(std::make_shared<ColoredWall>(rt, other, sf::Color{ 242, 131, 5 }));
		segment1.edges.push_back(std::make_shared<TexturedWall>(other, rb, metalTex));
		segment1.edges.push_back(std::make_shared<DoorWall>(rb, rt, 0));

		scene->addSegment(segment0);
		scene->addSegment(segment1);

		return scene;
	}

	int main() {
		unsigned int wWidth = 800;
		unsigned int wHeight = 600;

		float walkSpeed = 1.8f / 1000.0f;
		float rotateSpeed = 1.5f / 1000.0f;
		float ascendSpeed = 1.2f / 1000.0f;
		float hFOV = PI<float> * 0.36f;

		

		Camera camera{ sf::Vector3f{0.0f, 0.0f, 0.1f}, sf::Vector2f{0.0f, -1.0f}, 0, hFOV, (float)wWidth/(float)wHeight };
		

		RayCaster caster{ wWidth, wHeight, makeTestScene(), camera };

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
						caster.camera.goForward(walkSpeed * msElapsed);
						break;
					case sf::Keyboard::S:
						caster.camera.goForward(-walkSpeed * msElapsed);
						break;
					case sf::Keyboard::A:
						caster.camera.rotate(rotateSpeed * msElapsed);
						break;
					case sf::Keyboard::D:
						caster.camera.rotate(-rotateSpeed * msElapsed);
						break;
					case sf::Keyboard::Q:
						caster.camera.ascend(ascendSpeed * msElapsed);
						break;
					case sf::Keyboard::E:
						caster.camera.ascend(-ascendSpeed * msElapsed);
						break;
					}
				}
			}

			window.clear(sf::Color::Black);

			caster.render(window);
			info.setString("pos = [" + std::to_string(caster.camera.position.x) + "," + std::to_string(caster.camera.position.y) + "]\n"
						+  "dir = [" + std::to_string(caster.camera.direction.x) + "," + std::to_string(caster.camera.direction.y) + "]\n"
						+  "fps = " + std::to_string((int)(1000.0f / msElapsed)));

			window.draw(info);

			window.display();
		}

		return 0;
	}
}

int main() {
	return ps::main();
}
