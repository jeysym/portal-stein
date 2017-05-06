#include "Game.hpp"
#include <memory>
#include <filesystem>
#include <fstream>
#include <chrono>

#include "Math.hpp"
#include "LevelLoader.hpp"

namespace ps {

	sf::Font Game::textFont;
	sf::Texture Game::splashTex;
	sf::Texture Game::winTex;

	void Game::init()
	{
		// Floor ceiling must have its shaders compiled before the game starts.
		FloorCeiling::compileShaders();

		std::string pathToFont = "fonts\\OpenBaskerville-0.0.53.otf";
		bool success = textFont.loadFromFile(pathToFont);

		if (success == false)
			throw std::runtime_error("Font " + pathToFont + " could not be loaded!");
	
		if (!splashTex.loadFromFile("splash.png"))
			throw std::runtime_error("Texture splash.png could not be loaded!");

		if (!winTex.loadFromFile("win.png"))
			throw std::runtime_error("Texture win.png could not be loaded!");
	}

	Game::Game() : levels()
	{
		walkForce = 200.0f;
		ascendForce = 20.0f;
		rotateTorque = 100.0f;

		walkDragCoefficient1 = 70.0f;
		walkDragCoefficient2 = 10.0f;
		rotateDragCoefficient = 100.0f;

		infoEnabled = false;
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
				catch (UnexpectedTokenException & e) {
					std::string expectedType = getTokenString(e.expected);
					std::string actualType = getTokenString(e.actual);
					logFile << timeString << " : Syntax error in \"" << filePath << "\" | Line " << e.lineNumber << 
						": Expected: \"" << expectedType << "\", but \"" << actualType << "\" was read!" << std::endl;
				}
				catch (TexureLoadFailedException & e) {
					logFile << timeString << " : Error in \"" << filePath << "\" | Line " << e.lineNumber << 
						": Texture could not be loaded from \"" << e.path << "\"!" << std::endl;
				}
				catch (IdentifierException & e) {
					logFile << timeString << " : Error in \"" << filePath << "\" | Line " << e.lineNumber <<
						": Identifier \"" << e.id << "\" of type \"" << e.type << "\" : " << e.message << std::endl;
				}
				catch (OpenStringLiteralException & e) {
					logFile << timeString << " : Error in \"" << filePath << "\" | Line " << e.lineNumber <<
						": Ending quote of string literal was not found!" << std::endl;
				}
			}
		}
	}

	void processBasicEvent(sf::RenderWindow & window, sf::Event & e) {
		if (e.type == sf::Event::Closed)
			window.close();

		if (e.type == sf::Event::Resized) {
			// If window gets resized, the window gets recreated with different video mode
			auto width = window.getSize().x;
			auto height = window.getSize().y;

			window.create(sf::VideoMode{ width, height }, "Portal-stein");
		}
	}
	
	void Game::runSplashScreen(sf::RenderWindow & window)
	{
		sf::Sprite splash;
		splash.setTexture(splashTex);
		
		bool enterPressed = false;
		do {
			sf::Event e;
			while (window.pollEvent(e)) {
				processBasicEvent(window, e);

				if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return)
					enterPressed = true;
			}

			float sX = (float)window.getSize().x / splashTex.getSize().x;
			float sY = (float)window.getSize().y / splashTex.getSize().y;
			splash.setScale(sX, sY);

			window.draw(splash);
			window.display();
		} while (!enterPressed && window.isOpen());
	}

	void Game::runGameplay(Level & level, sf::RenderWindow & window)
	{
		auto && scene = level.makeScene();
		// get the clock object
		sf::Clock clock;
		float deltaTime = 0.001f;

		bool finish = false;
		do
		{
			// measure the time elapsed from the last draw
			deltaTime = clock.getElapsedTime().asSeconds();
			clock.restart();

			sf::Event e;
			while (window.pollEvent(e)) {
				processBasicEvent(window, e);

				// Toggle info drawing on pressing F1
				if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F1)
					infoEnabled = !infoEnabled;
			}

			processGameInput(window, scene, deltaTime);
			simulateDrag(scene);
			scene.camera.simulate(deltaTime);

			window.clear(sf::Color::Black);			// clear the window
			caster.render(window, scene);			// render the game
			
			if (infoEnabled)
				drawInfo(window, scene, deltaTime);		// draw some additional info like position, direction, fps

			Segment & cameraSegment = scene.getSegment(scene.camera.getSegmentId());
			finish = cameraSegment.finish;

			window.display();
		} while (finish == false && window.isOpen());
	}

	void Game::runWinScreen(sf::RenderWindow & window)
	{
		sf::Sprite splash;
		splash.setTexture(winTex);

		while (window.isOpen()) {
			sf::Event e;
			while (window.pollEvent(e))
				processBasicEvent(window, e);

			float sX = (float)window.getSize().x / splashTex.getSize().x;
			float sY = (float)window.getSize().y / splashTex.getSize().y;
			splash.setScale(sX, sY);

			window.draw(splash);
			window.display();
		} 
	}

	void Game::processGameInput(sf::RenderWindow & window, Scene & scene, float deltaTime)
	{
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
	
	void Game::simulateDrag(Scene & scene) {
		auto speed = scene.camera.getSpeed();
		auto angularSpeed = scene.camera.getAngularSpeed();

		auto walkDrag = -1.0f * walkDragCoefficient1 * speed - walkDragCoefficient2 * norm(speed) * speed;
		auto rotateDrag = -1.0f * angularSpeed * rotateDragCoefficient;

		scene.camera.applyForce(walkDrag);
		scene.camera.applyTorque(rotateDrag);
	}

	void Game::drawInfo(sf::RenderTarget & window, Scene & scene, float secondsElapsed)
	{
		sf::Text info{ "N/A", Game::textFont };
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

		runSplashScreen(window);

		for (Level & level : levels)
			runGameplay(level, window);

		runWinScreen(window);
	}

}
