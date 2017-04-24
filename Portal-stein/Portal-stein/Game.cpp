#include "Game.hpp"
#include <memory>
#include <filesystem>
#include <fstream>
#include <chrono>

#include "Math.hpp"
#include "LevelLoader.hpp"

namespace ps {

	//sf::Texture wallTex;
	//sf::Texture metalTex;
	//sf::Texture floorTex;
	//sf::Texture ceilingTex;

	//std::shared_ptr<Scene> makeTestScene() {
	//	wallTex.loadFromFile("levels\\textures\\wall.bmp");
	//	metalTex.loadFromFile("levels\\textures\\metal.bmp");
	//	floorTex.loadFromFile("levels\\textures\\floor.bmp");
	//	ceilingTex.loadFromFile("levels\\textures\\ceiling.bmp");

	//	sf::Vector2f a(0.0f, 0.0f);
	//	sf::Vector2f b(0.0f, 10.0f);
	//	sf::Vector2f c(10.0f, 10.0f);
	//	sf::Vector2f d(10.0f, 0.0f);

	//	sf::Vector2f offset(40.0f, 0.0f);
	//	sf::Vector2f e = a + offset;
	//	sf::Vector2f f = b + offset;
	//	sf::Vector2f g = c + offset;
	//	sf::Vector2f h = d + offset;

	//	Floor floor(sf::Color::White, &floorTex);
	//	Ceiling ceiling(sf::Color::White, &ceilingTex);

	//	SegmentBuilder segment0( floor, ceiling );
	//	segment0.addWall(PortalWall(a, b, sf::Color::Green));
	//	segment0.addWall(PortalWall(b, c, sf::Color::White, &wallTex));
	//	segment0.addWall(makeWallPortalWall(LineSegment(c, d), LineSegment(g, f), 1));
	//	segment0.addWall(PortalWall(d, a, sf::Color::Green));

	//	SegmentBuilder segment1(floor, ceiling);
	//	segment1.addWall(PortalWall(e, f, sf::Color::Yellow));
	//	segment1.addWall(makeWallPortalWall(LineSegment(f, g), LineSegment(d, c), 0));
	//	segment1.addWall(PortalWall(g, h, sf::Color::Yellow));
	//	segment1.addWall(PortalWall(h, e, sf::Color::Yellow));

	//	auto result = std::make_shared<Scene>(
	//		ObjectInScene(sf::Vector3f(1.0f, 1.0f, 0.5f), sf::Vector2f(1.0f, 1.0f), 0)
	//	);
	//	
	//	result->addSegment(segment0.finalize());
	//	result->addSegment(segment1.finalize());
	//	return result;
	//}

	void Game::simulateDrag(Scene & scene) {
		auto speed = scene.camera.getSpeed();
		auto angularSpeed = scene.camera.getAngularSpeed();

		auto walkDrag = -1.0f * walkDragCoefficient1 * speed - walkDragCoefficient2 * norm(speed) * speed;
		auto rotateDrag = -1.0f * angularSpeed * rotateDragCoefficient;

		scene.camera.applyForce(walkDrag);
		scene.camera.applyTorque(rotateDrag);
	}

	void Game::processUserInput(sf::RenderWindow & window, Scene & scene, float deltaTime)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// dispatch all the events in the queue

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized) {
				// If window gets resized, the window gets recreated with different video mode
				auto width = window.getSize().x;
				auto height = window.getSize().y;

				window.create(sf::VideoMode{ width, height }, "Portal-stein");
			}
		}

		auto direction = ps::toVector3(scene.camera.getDirection());
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			scene.camera.applyForce(walkForce * direction);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			scene.camera.applyForce(-1.0f * walkForce * direction);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			scene.camera.applyTorque(rotateTorque);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			scene.camera.applyTorque(-1.0f * rotateTorque);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			scene.camera.applyForce(ascendForce * sf::Vector3f(0.0f, 0.0f, 1.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			scene.camera.applyForce(ascendForce * sf::Vector3f(0.0f, 0.0f, -1.0f));
		}
	}

	sf::Font textFont;

	void Game::loadFont()
	{
		std::string pathToFont = "fonts\\OpenBaskerville-0.0.53.otf";
		bool success = textFont.loadFromFile(pathToFont);

		if (success == false) 
			throw std::runtime_error("Font " + pathToFont + " could not be loaded!");
	}

	Game::Game() : levels()
	{
		// Floor ceiling must have its shaders compiled before the game starts.
		FloorCeiling::compileShaders();
		
		loadFont();

		walkForce = 200.0f;
		ascendForce = 20.0f;
		rotateTorque = 100.0f;

		walkDragCoefficient1 = 70.0f;
		walkDragCoefficient2 = 10.0f;
		rotateDragCoefficient = 100.0f;
	}

	void drawInfo(sf::RenderTarget & window, Scene & scene, float secondsElapsed)
	{
		sf::Text info{ "N/A", textFont };
		info.setFillColor(sf::Color::Black);
		info.setStyle(sf::Text::Bold);

		auto position = scene.camera.getPosition();
		auto direction = scene.camera.getDirection();
		auto segmentId = scene.camera.getSegmentId();

		info.setString(
			"pos = (" + std::to_string(position.x) + "," + std::to_string(position.y) + "," + std::to_string(position.z) + ")\n" +
			"dir = (" + std::to_string(direction.x) + "," + std::to_string(direction.y) + ")\n" +
			"segment = " + std::to_string(segmentId) + "\n" +
			"fps = " + std::to_string((int)(1.0f / secondsElapsed))
		);

		window.draw(info);
	}

	void Game::run()
	{
		RayCaster caster;
				
		unsigned int wWidth = 800;
		unsigned int wHeight = 600;
		sf::RenderWindow window( sf::VideoMode( wWidth, wHeight ), "Portal-stein" );
		window.setVerticalSyncEnabled(true);

		for (Level & level : levels) {
			auto && scene = level.makeScene();

			// get the clock object
			sf::Clock clock;
			float deltaTime = 0.001f;

			while (window.isOpen())
			{
				// measure the time elapsed from the last draw
				deltaTime = clock.getElapsedTime().asSeconds();
				clock.restart();

				processUserInput(window, scene, deltaTime);
				simulateDrag(scene);
				scene.camera.simulate(deltaTime);

				window.clear(sf::Color::Black);			// clear the window
				caster.render(window, scene);			// render the game
				drawInfo(window, scene, deltaTime);		// draw some additional info like position, direction, fps

				window.display();
			}
		}
	}

	void Game::loadLevels(const std::string & levelDirPath) {
		using namespace std::experimental::filesystem;
		using namespace std::chrono;

		std::ofstream logFile;
		logFile.open("log.txt", std::ios_base::app);	// opens file in append mode

		for (const path & file : directory_iterator(levelDirPath)) {
			if (is_regular_file(file)) {
				std::string filePath = file.string();

				time_point<system_clock> time = system_clock::now();
				std::time_t ttime = system_clock::to_time_t(time);
				std::string timeString = std::string(std::ctime(&ttime)); // TODO : this may be unsafe
				timeString.pop_back();	// ctime inserts a line-feed character at the end => pop it

				try {
					std::ifstream fileStream;
					fileStream.open(filePath);

					LevelLoader loader(fileStream);
					levels.push_back(loader.loadLevel());

					logFile << timeString << " : \"" << filePath << "\" parsed successfuly!" << std::endl;

					fileStream.close();
				}
				catch (UnexpectedTokenException e) {
					std::string expectedType = getTokenString(e.expected);
					std::string actualType = getTokenString(e.actual);
					logFile << timeString << " : Syntax error in \"" << filePath << "\" | Line " << e.lineNumber << 
						": Expected: \"" << expectedType << "\", but \"" << actualType << "\" was read!" << std::endl;
				}
				catch (TexureLoadFailedException e) {
					logFile << timeString << " : Error in \"" << filePath << "\" | Line " << e.lineNumber << 
						": Texture could not be loaded from \"" << e.path << "\"!" << std::endl;
				}
				catch (IdentifierException e) {
					logFile << timeString << " : Error in \"" << filePath << "\" | Line " << e.lineNumber <<
						": Identifier \"" << e.id << "\" of type \"" << e.type << "\" : " << e.message << std::endl;
				}
			}
		}
	}

}
