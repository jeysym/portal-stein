#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "LevelLoader.hpp"

namespace ps {

	int main() {
		Game myGame;
		//myGame.loadLevels()
		myGame.run();

		return 0;
	}
}

int main() {
	return ps::main();
}
