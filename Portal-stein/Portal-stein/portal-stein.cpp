#include <iostream>
#include <SFML/Graphics.hpp>

int main(int argc, char * * args) {
	int width = 800;
	int height = 600;
	
	sf::RenderWindow window(sf::VideoMode(width, height), "Portal-stein");

	sf::Font comicSans;
	comicSans.loadFromFile("Comic_Sandchez.ttf");

	sf::Text wipText{ "Work in progress...", comicSans, 40 };

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		

		float recWid = width / 10.0f;
		float recHei = height;
		for (int i = 0; i < 10; ++i) {
			sf::RectangleShape rectangle{ sf::Vector2f{recWid, recHei} };
			sf::Uint8 red = i * 255 / 9;
			rectangle.setFillColor(sf::Color{ red, (sf::Uint8)(255 - red), 0 });
			rectangle.move(sf::Vector2f{(float)( i * recWid), 0.0f });
			window.draw(rectangle);
		}

		window.draw(wipText);

		window.display();
	}

	return 0;
}