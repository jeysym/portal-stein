#pragma once
#ifndef PS_GAME_INCLUDED
#define PS_GAME_INCLUDED
#include "RayCaster.hpp"
#include "Level.hpp"
#include <vector>

namespace ps {

	class Game {
	private:
		std::vector<Level> levels;

		void simulateDrag(Scene & scene);
		void processUserInput(sf::RenderWindow & window, Scene & caster, float deltaTime);
		void loadFont();

	public:
		Game();

		float walkForce;
		float ascendForce;
		float rotateTorque;

		float walkDragCoefficient1;
		float walkDragCoefficient2;
		float rotateDragCoefficient;

		void run();
		void loadLevels(const std::string & levelDirPath);
	};
}

#endif // !PS_GAME_INCLUDED
