#include "Game.hpp"
#include <memory>
#include "RayCaster.hpp"
#include "Math.hpp"

namespace ps {

	std::shared_ptr<Scene> makeTestScene() {
		auto result = std::make_shared<Scene>();

		sf::Texture wallTex;
		wallTex.loadFromFile("H:\\MyData\\Textures\\wall.bmp");
		sf::Texture metalTex;
		metalTex.loadFromFile("H:\\MyData\\Textures\\metal.bmp");
		sf::Texture floorTex;
		floorTex.loadFromFile("H:\\MyData\\Textures\\floor.bmp");
		sf::Texture ceilingTex;
		ceilingTex.loadFromFile("H:\\MyData\\Textures\\ceiling.bmp");

		floorPtr floor = std::make_shared<TexturedFloor>(floorTex);
		ceilingPtr ceiling = std::make_shared<TexturedCeiling>(ceilingTex);

		sf::Vector2f a{ 0.0f, 2.0f };
		sf::Vector2f b{ 2.0f, 2.0f };
		sf::Vector2f c{ 12.0f, 2.0f };
		sf::Vector2f d{ 0.0f, 0.0f };
		sf::Vector2f e{ 10.0f, 0.0f };
		sf::Vector2f f{ 12.0f, 0.0f };

		Segment room0{ floor, ceiling };
		room0.edges.push_back(std::make_unique<WallPortalWall>(LineSegment{ a, b }, LineSegment{ e, f }, 0));
		room0.edges.push_back(std::make_unique<TexturedWall>(b, c, wallTex));
		room0.edges.push_back(std::make_unique<TexturedWall>(c, f, wallTex));
		room0.edges.push_back(std::make_unique<WallPortalWall>(LineSegment{ f, e }, LineSegment{ b, a }, 0));
		room0.edges.push_back(std::make_unique<TexturedWall>(e, d, metalTex));
		room0.edges.push_back(std::make_unique<TexturedWall>(d, a, metalTex));

		result->addSegment(room0);

		return result;
	}

	void Game::run()
	{
		unsigned int wWidth = 400;
		unsigned int wHeight = 300;

		float walkForce = 500.0f;
		float ascendForce = 30.0f;
		float rotateTorque = 80.0f;

		Camera camera{ sf::Vector3f{ 4.0f, 1.0f, 0.5f }, sf::Vector2f{ 0.62f, -0.7f }, 0 };
		camera.setFOV(PI<float> * 0.5f, 4.0f / 3.0f);
		auto scene = makeTestScene();

		RayCaster caster{ camera };
		caster.setScene(scene);

		sf::RenderWindow window{ sf::VideoMode{ wWidth, wHeight }, "Portal-stein" };
		window.setVerticalSyncEnabled(true);

		sf::Font font;
		font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
		sf::Text info{ "NA", font };
		info.setFillColor(sf::Color::Black);

		RigidBody player{ 75.0f };

		// get the clock object
		sf::Clock clock;

		
		while (window.isOpen())
		{
			// measure the time elapsed from the last draw
			float secondsElapsed = clock.getElapsedTime().asSeconds();
			clock.restart();

			sf::Event event;
			while (window.pollEvent(event))
			{
				// dispatch all the events in the queue

				if (event.type == sf::Event::Closed)
					window.close();
			}

			sf::Vector3f cameraDirection = toVector3(caster.camera.getDirection());
			sf::Vector3f upDirection{ 0.0f, 0.0f, 1.0f };

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				player.applyForce(walkForce * cameraDirection);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				player.applyForce(-1.0f * walkForce * cameraDirection);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				player.applyTorque(rotateTorque);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				player.applyTorque(-1.0f * rotateTorque);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				player.applyForce(ascendForce * upDirection);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
				player.applyForce(-1.0f * ascendForce * upDirection);
			}

			float playerSpeed = norm(player.getCurrentVelocity());
			float k1 = 80.0f;
			float k2 = 30.0f;
			float dragMagnitude = k1 * playerSpeed + k2 * playerSpeed * playerSpeed;
			player.applyForce(-1.0f * dragMagnitude * player.getCurrentVelocity());

			float rk = 80.0f;
			player.applyTorque(-1.0f * rk * player.getAngularSpeed());

			player.Simulate(secondsElapsed);

			auto positionChange = secondsElapsed * player.getCurrentVelocity();
			auto rotationChange = secondsElapsed * player.getAngularSpeed();
			caster.moveCamera(positionChange);
			caster.rotateCamera(rotationChange);

			window.clear(sf::Color::Black);
			caster.render(window);

			auto position = caster.camera.getPosition();
			auto direction = caster.camera.getDirection();
			auto segmentId = caster.camera.getSegmentId();
			info.setString(
				"pos = [" + std::to_string(position.x) + "," + std::to_string(position.y) + "]\n" +
				"dir = [" + std::to_string(direction.x) + "," + std::to_string(direction.y) + "]\n" +
				"segment = " + std::to_string(segmentId) + "\n" +
				"fps = " + std::to_string((int)(1.0f / secondsElapsed))
			);

			window.draw(info);

			window.display();
		}
	}

}
