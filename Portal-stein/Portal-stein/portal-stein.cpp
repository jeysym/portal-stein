#include <SFML/Graphics.hpp>
#include "Game.hpp"

namespace ps {

	int main() {
		Game myGame;
		
		myGame.run();

		return 0;
	}
}

int main() {
	return ps::main();
}
