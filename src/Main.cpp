
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace std;
using namespace sf;


int main()
{
	RenderWindow window(VideoMode(320, 480), "Tetris");

	Texture texture;
	texture.loadFromFile("src/images/tiles.png");

	Sprite sprite(texture);
	sprite.setTextureRect(IntRect(0,0,18,18));

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if(e.type == Event::Closed){
				window.close();
			}
		}
		window.clear(Color::White);
		window.draw(sprite);
		window.display();
	}


}
