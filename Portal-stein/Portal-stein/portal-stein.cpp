#include <SFML/Graphics.hpp>
#include "Game.hpp"

namespace ps {

	int main() {
		Game myGame;

		std::string levelDirPath = "levels";	// levels are kept in ./levels directory
		myGame.loadLevels(levelDirPath);		// game loads the levels from some directory
		myGame.run();							// game executes 

		return 0;
	}
}

int main() {
	return ps::main();
}
