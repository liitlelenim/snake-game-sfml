
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>




int main()
{
	struct SnakeBlock {
		char x=-1, y=-1;
	} snake[2500];
	
	enum direction {up,down,right,left};
	
	const unsigned short WINDOW_SIZE = 640;
	const unsigned short CELL_SIZE = 16;
	const unsigned short MAX_SNAKE_SIZE = (WINDOW_SIZE / CELL_SIZE) ^ 2;


	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Snake!", sf::Style::Titlebar | sf::Style::Close);
	
	sf::Texture t_background, t_snake, t_fruit;

	if (!t_background.loadFromFile("assets/background.png"))
	{
		std::cout << "sackground.png not found...";
		return EXIT_FAILURE;
	};
	if (!t_snake.loadFromFile("assets/snake.png"))
	{
		std::cout << "snake.png not found...";
		return EXIT_FAILURE;
	};
	if (!t_fruit.loadFromFile("assets/fruit.png"))
	{
		std::cout << "fruit.png not found...";
		return EXIT_FAILURE;
	};
	sf::Sprite s_background(t_background), s_snake(t_snake), s_fruit(t_fruit);
	sf::Clock deltaClock;
	int tickDuration=50;//In Miliseconds
	bool addLength=false;
	char fruitX=24 ,fruitY=24;
	//Snake initialization
	for (int i = 0; i < 5; i++)
	{
		snake[i].x = 0;
		snake[i].y = 5 - i;
	}
	srand(time(NULL));
	window.setFramerateLimit(60);
	direction currentDirection = direction::down;
	while (window.isOpen())
	{
		//-----------EVENT HANDLING-----------//
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Up:
					{
						if (currentDirection != direction::down)
						{
							currentDirection = direction::up;
						}
						break;
					}
					case sf::Keyboard::Down:
					{
						if (currentDirection != direction::up)
						{
							currentDirection = direction::down;
						}
						break;
					}
					case sf::Keyboard::Left:
					{
						if (currentDirection != direction::right)
						{
							currentDirection = direction::left;
						}
						break;
					}
					case sf::Keyboard::Right:
					{
						if (currentDirection != direction::left)
						{
							currentDirection = direction::right;
						}
						break;
					}
				}
			}
		}
		//----------------TICK----------------//
		if(deltaClock.getElapsedTime().asMilliseconds()>tickDuration)
		{
			int possibleTailIndex;
			SnakeBlock possibleTailPosition;
			for (int i = MAX_SNAKE_SIZE-1; i > 0; i--)
			{
				if (snake[i].x != -1 && snake[i].y != -1)
				{

					snake[i] = snake[i - 1];
				}
				else
				{
					if (snake[i - 1].x!=-1)
					{
						possibleTailIndex = i;
						possibleTailPosition = snake[i-1];
					}
				}
			}

			switch (currentDirection)
			{
				case direction::up: {
					snake[0].y--;
					break;
				}
				case direction::down: {
					snake[0].y++;
					break;
				}
				case direction::left: {
					
					snake[0].x--;
					break;
				}
				case direction::right: {
					snake[0].x++;
					break;
				}
			}

			//Behaviour of snake when it's going outside of the map
			if (snake[0].x < 0)
			{
				snake[0].x = (WINDOW_SIZE/CELL_SIZE)-1;
			}
			if (snake[0].x > (WINDOW_SIZE / CELL_SIZE)-1)
			{
				snake[0].x = 0;
			}
			if (snake[0].y < 0)
			{
				snake[0].y = (WINDOW_SIZE / CELL_SIZE) - 1;
			}
			if (snake[0].y > (WINDOW_SIZE / CELL_SIZE) - 1)
			{
				snake[0].y = 0;
			}

			bool cut=false;
			for (int i = 1; i < MAX_SNAKE_SIZE; i++)
			{

				if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
				{
					cut = true;
				}
				if (cut)
				{
					snake[i].x = -1;
					snake[i].y = -1;
				}
			}

			if (snake[0].x == fruitX && snake[0].y == fruitY)
			{
				snake[possibleTailIndex] = possibleTailPosition;
				
				bool freeCell = true;

				do {
					fruitX = rand() % (WINDOW_SIZE / CELL_SIZE);
					fruitY = rand() % (WINDOW_SIZE / CELL_SIZE);
					freeCell = true;
					for (int i = 1; i < MAX_SNAKE_SIZE; i++)
					{
						if (snake[i].x == fruitX && snake[i].y == fruitY)
						{
							freeCell = false;
						}
					}
				} while (!freeCell);
				
			}
		
			deltaClock.restart();
		}
	
		//-----------DRAWING FRAME------------//
		window.clear();
		for (int x = 0; x < WINDOW_SIZE; x += CELL_SIZE)
		{
			for (int y = 0; y < WINDOW_SIZE; y += CELL_SIZE)
			{
				s_background.setPosition(x,y);
				window.draw(s_background);
			}
		}
		for (int i = 0; i < 2500; i++)
		{
			if (snake[i].x == -1)
			{
				continue;
			}
			s_snake.setPosition(snake[i].x*CELL_SIZE, snake[i].y*CELL_SIZE);
			window.draw(s_snake);
		}
		s_fruit.setPosition(fruitX*CELL_SIZE, fruitY*CELL_SIZE);
		window.draw(s_fruit);
		window.display();
		
	}
	return EXIT_SUCCESS;
}