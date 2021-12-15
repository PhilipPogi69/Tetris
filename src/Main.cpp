#include <MMsystem.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;
using namespace sf;

int GameOver(int s);
void wallkick(bool rotate);

const int column = 20;
const int row = 10;
bool check();

int field[column][row] = { 0 };
int block;

struct Point
{int x,y;} a[4]={{1,0},{1,1},{1,2},{1,3}}, b[4];

Point figures[7][4] =
{
 {{1,0},{1,1},{1,2},{1,3}},// I
 {{0,1},{1,1},{1,2},{2,2}},
 {{0,1},{1,1},{2,1},{1,2}},
 {{0,0},{1,0},{1,1},{1,2}},
 {{0,1},{1,1},{0,2},{1,2}},
 {{0,2},{1,1},{1,2},{2,1}},
 {{1,0},{1,1},{1,2},{2,0}} // z
};

bool check()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= row || a[i].y >= column)
			return 0;
		else if (field[a[i].y][a[i].x])
			return 0;
	}
	return 1;
}

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(320, 480), " TETRIS ", sf::Style::Close | sf::Style::Titlebar);

	Texture t1, t2, t3;
	t1.loadFromFile("src/images/tiles.png");
	t2.loadFromFile("src/images/background.jpg");
	t3.loadFromFile("src/images/frame.png");

	Sprite s(t1), background(t2), frame(t3);

	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;
	float timer = 0, delay = 0.3;
	int scoreCount = 0, lineClear = 0;

	Clock clock;
	//set up text font
	sf::Text score, line;
	sf::Font font;
	font.loadFromFile("Forwa_font.TTF");
	score.setFont(font);
	score.setCharacterSize(15);
	score.setFillColor(sf::Color::Black);
	score.setPosition(10, 450);
	sf::Vector2<float> score_scale(1.5f, 1.5f);
	score.setScale(score_scale);

	font.loadFromFile("Forwa_font.TTF");
	line.setFont(font);
	line.setCharacterSize(15);
	line.setFillColor(sf::Color::Black);
	line.setPosition(10, 410);
	sf::Vector2<float> line_scale(1.5f, 1.5f);
	line.setScale(line_scale);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Clock clock;

		Music GameBGM;

		if (!GameBGM.openFromFile("src/music/GameMusic.ogg"))
		{
			std::cout << "ERROR" << std::endl;
		}

		GameBGM.play();
		GameBGM.setLoop(true);
		GameBGM.setVolume(35);

		while (window.isOpen())
		{
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			Event e;
			while (window.pollEvent(e))
			{
				if (e.type == Event::Closed)
					window.close();

				if (e.type == Event::KeyPressed)
				{
					if (e.key.code == Keyboard::Up)
						rotate = true;
					else if (e.key.code == Keyboard::Left)
						dx = -1;
					else if (e.key.code == Keyboard::Right)
						dx = 1;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
				delay = 0.05;

			//// <- Move -> ///
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].x += dx;
			}
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];

			//////Rotate//////
			wallkick(rotate);

			///////Tick//////
			if (timer > delay)
			{
				for (int i = 0; i < 4; i++)
				{
					b[i] = a[i];
					a[i].y += 1;
				}

				if (!check())
				{
					for (int i = 0; i < 4; i++)
						field[b[i].y][b[i].x] = colorNum;

					colorNum = 1 + rand() % 6;
					int n = rand() % 7;
					block = n;
					for (int i = 0; i < 4; i++)
					{
						a[i].x = figures[n][i].x;
						a[i].y = figures[n][i].y;
					}
				}
				timer = 0;
			}

			char temp[256];
			sprintf_s(temp, "Scores: %d", scoreCount);
			score.setString(temp);

			sprintf_s(temp, "LIne: %d", lineClear);
			line.setString(temp);

			// Scoring and total line clear
			for (int c = 19; c > 0; c--)
			{
				if (field[c][0] != 0 && field[c][1] != 0 && field[c][2] != 0 && field[c][3] != 0 && field[c][4] != 0 && field[c][5] != 0 && field[c][6] != 0 && field[c][7] != 0 && field[c][8] != 0 && field[c][9] != 0)

				{
					scoreCount += 10;
					lineClear++;
				}
			}
			///////check lines//////////
			int k = column - 1;
			for (int i = column - 1; i > 0; i--)
			{
				int count = 0;
				for (int j = 0; j < row; j++)
				{
					if (field[i][j])
						count++;
					field[k][j] = field[i][j];
				}
				if (count < row)
					k--;
			}

			//If game over, then close application
			for (int i = 0; i < row; i++)
			{
				if (field[1][i] != 0)
				{
					window.close();
				}
			}

			dx = 0;
			rotate = 0;
			delay = 1;

			/////////draw//////////
			window.clear(Color::White);
			window.draw(background);

			for (int i = 0; i < column; i++)
			{
				for (int j = 0; j < row; j++)
				{
					if (field[i][j] == 0)
						continue;
					s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
					s.setPosition(j * 18, i * 18);
					s.move(28, 31); //offset
					window.draw(s);
				}
			}

			for (int i = 0; i < 4; i++)
			{
				s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				s.setPosition(a[i].x * 18, a[i].y * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}
			window.draw(line);
			window.draw(score);
			window.draw(frame);
			window.display();
		}
		//Displays Game Over Screen & Scores
		GameBGM.stop();
		GameOver(scoreCount);
		return 0;
	}
}

void wallkick(bool rotate)
{
	if (rotate)
	{
		Point p = a[1]; //center of rotation
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}
		if (!check())
			for (int i = 0; i < 4; i++)
			{
				switch (block)
				{
					case 0:
						if (a[i].x > 8)
						{
							a[i].x -= 4;
						}
						a[i].x += 2;
						break;
					case 1:
						if (a[i].x > 7)
						{
							a[i].x -= 2;
						}
						a[i].x += 1;
						break;
					case 2:
						if (a[i].x > 7)
						{
							a[i].x -= 2;
						}
						a[i].x += 1;
						break;
					case 3:
						if (a[i].x > 7)
						{
							a[i].x -= 2;
						}
						a[i].x += 1;
						break;
					case 4:
						if (a[i].x > 7)
						{
							a[i].x -= 2;
						}
						a[i].x += 1;
						break;
					case 5:
						if (a[i].x > 7)
						{
							a[i].x -= 2;
						}
						a[i].x += 1;
						break;
					default:
						break;
				}
			}
	}
}

// GameOver Function
int GameOver(int s)
{

	sf::Text score;
	sf::Font font;
	font.loadFromFile("Oswald-Heavy.ttf");
	score.setFont(font);
	score.setCharacterSize(40);
	score.setFillColor(sf::Color::White);
	score.setPosition(90, 50);
	sf::Vector2<float> score_scale(1.5f, 1.5f);
	score.setScale(score_scale);
	char temp[256];
	sprintf_s(temp, " < GAME OVER > \nYOUR SCORE : %d", s);
	score.setString(temp);

	sf::RenderWindow window(sf::VideoMode(600, 300), "GAME OVER!", sf::Style::Close | sf::Style::Titlebar);

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}
		window.draw(score);
		window.display();
	}

	return 0;
}
