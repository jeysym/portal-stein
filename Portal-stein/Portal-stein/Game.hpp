#pragma once
#ifndef PS_GAME_INCLUDED
#define PS_GAME_INCLUDED
#include "RayCaster.hpp"
#include "Level.hpp"
#include <vector>

namespace ps {

	/// Class representing the game.
	class Game {
	private:
		static sf::Font textFont;				///< Font used for writing various texts in the game.
		static sf::Texture splashTex;			///< Texture for splash screen.
		static sf::Texture winTex;				///< Texture for win screen.

		bool infoEnabled;
		std::vector<Level> levels;
		RayCaster caster;

		void simulateDrag(Scene & scene);
		void processGameInput(sf::RenderWindow & window, Scene & caster, float deltaTime);
		void drawInfo(sf::RenderTarget & window, Scene & scene, float secondsElapsed);

		/// Runs part of the game, when splash screen is showed.
		void runSplashScreen(sf::RenderWindow & window);
		/// Runs specific level.
		void runGameplay(Level & level, sf::RenderWindow & window);
		/// Runs final part of the game, when win screen is showed.
		void runWinScreen(sf::RenderWindow & window);

	public:
		float walkForce;	///< Force induced by pressing movement buttons. (W/S)
		float ascendForce;	///< Force induced by pressing ascend buttons. (Q/E)
		float rotateTorque;	///< Torque induced by pressing rotate button. (A/D)

		// these coefficients are used when calculating drag force relative to speed (and its square) of the camera
		float walkDragCoefficient1;
		float walkDragCoefficient2;
		float rotateDragCoefficient;

		/// Loads the font and the textures used in Game class. This must be called prior to using the Game class.
		static void init();
		Game();

		/// Loads all the levels in specified directory.
		/// \param levelDirPath Directory where to look for level files.
		void loadLevels(const std::string & levelDirPath);
		/// Runs the game. 
		/// It creates the window, shows splash screen, shows the levels (and enables the user to interact with it), and then shows the win screen.
		void run();
	};
}

#endif // !PS_GAME_INCLUDED
