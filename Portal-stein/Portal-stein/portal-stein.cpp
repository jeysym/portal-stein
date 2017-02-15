#include <SFML/Graphics.hpp>
#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {

	std::shared_ptr<Scene> makeTestScene() {
		sf::Vector2f lt{ -5.0f, 5.0f };
		sf::Vector2f lb{ -5.0f, -5.0f };
		sf::Vector2f rt{ 5.0f, 5.0f };
		sf::Vector2f rb{ 5.0f, -5.0f };

		sf::Vector2f other{ 13.0f, 0.0f };

		auto scene = std::make_shared<Scene>();
		floorPtr floor = std::make_shared<ColoredFloor>(sf::Color::Green);
		ceilingPtr ceiling = std::make_shared<ColoredCeiling>(sf::Color::Cyan);

		std::unique_ptr<Portal> door1 = std::make_unique<Door>(1);
		std::unique_ptr<Portal> door2 = std::make_unique<Door>(0);

		Segment segment0{ floor, ceiling };
		segment0.edges.push_back(std::make_shared<ColoredEdge>(lt, rt, sf::Color::Blue));
		segment0.edges.push_back(std::make_shared<ColoredEdge>(rt, rb, sf::Color::White, door1));
		segment0.edges.push_back(std::make_shared<ColoredEdge>(rb, lb, sf::Color::Red));
		segment0.edges.push_back(std::make_shared<ColoredEdge>(lb, lt, sf::Color::Yellow));

		Segment segment1{ floor, ceiling };
		segment1.edges.push_back(std::make_shared<ColoredEdge>(rt, other, sf::Color{ 242, 131, 5 }));
		segment1.edges.push_back(std::make_shared<ColoredEdge>(other, rb, sf::Color{ 0, 20, 240 }));
		segment1.edges.push_back(std::make_shared<ColoredEdge>(rb, rt, sf::Color::White, door2));

		scene->segments.push_back(segment0);
		scene->segments.push_back(segment1);

		return scene;
	}

	int main() {
		unsigned int wWidth = 800;
		unsigned int wHeight = 600;

		float walkSpeed = 1.8 / 1000.0;
		float rotateSpeed = 1.5 / 1000.0;
		float hFOV = (float)PI * 0.3f;

		

		Camera camera{ sf::Vector3f{0.0f, 0.0f, 0.5f}, sf::Vector2f{0.0f, -1.0f}, 0, hFOV, (float)wWidth/(float)wHeight };
		

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
			float msElapsed = clock.getElapsedTime().asMilliseconds();
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
					}
				}
			}

			window.clear(sf::Color::Black);

			caster.render(window);
			info.setString("pos = [" + std::to_string(caster.camera.position.x) + "," + std::to_string(caster.camera.position.y) + "]\n"
						+  "dir = [" + std::to_string(caster.camera.direction.x) + "," + std::to_string(caster.camera.direction.y) + "]");

			window.draw(info);

			window.display();
		}

		return 0;
	}
}

int main() {
	return ps::main();
}
