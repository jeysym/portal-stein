#pragma once
#ifndef PS_GAME_INCLUDED
#define PS_GAME_INCLUDED
#include "RayCaster.hpp"

namespace ps {

	class Game {
	private:
		
		/*
		void applyDragToCamera(RayCaster & caster);
		void drawInfo(sf::RenderWindow & window, RayCaster & caster, float secondsElapsed);
		*/

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
		void loadLevels();
	};
}

#endif // !PS_GAME_INCLUDED
