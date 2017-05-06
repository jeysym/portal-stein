#pragma once
#ifndef PS_GAME_INCLUDED
#define PS_GAME_INCLUDED
#include "RayCaster.hpp"
#include "Level.hpp"
#include <vector>

namespace ps {

	class Game {
	private:
		static sf::Font textFont;
		static sf::Texture splashTex, winTex;

		bool infoEnabled;
		std::vector<Level> levels;
		RayCaster caster;

		void simulateDrag(Scene & scene);
		void processGameInput(sf::RenderWindow & window, Scene & caster, float deltaTime);
		void drawInfo(sf::RenderTarget & window, Scene & scene, float secondsElapsed);

		void runSplashScreen(sf::RenderWindow & window);
		void runGameplay(Level & level, sf::RenderWindow & window);
		void runWinScreen(sf::RenderWindow & window);

	public:
		float walkForce;
		float ascendForce;
		float rotateTorque;

		float walkDragCoefficient1;
		float walkDragCoefficient2;
		float rotateDragCoefficient;


		static void init();
		Game();

		void loadLevels(const std::string & levelDirPath);
		void run();
	};
}

#endif // !PS_GAME_INCLUDED
