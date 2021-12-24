#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
using namespace std;

using namespace sf;
const int DisplayX = 800;
const int DisplayY = 600;
const int EnemyX = 56;
const int EnemyY = 50;
const int EnemyPosX = DisplayX - EnemyX;

void menu(RenderWindow& window) 
{

	Texture menuTexture1, menuTexture2, aboutTexture;
	menuTexture1.loadFromFile("images//play.jpg");
	menuTexture2.loadFromFile("images/exit.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2);
	bool isMenu = true;
	int menuNum = 0;
	menu1.setPosition(318, 250);
	menu2.setPosition(318, 325);
	while (isMenu == true)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			menuNum = 0;
			window.clear(Color(255,255,255));
			if (IntRect(318, 250, 518, 50).contains(Mouse::getPosition(window))) 
			{ 
				menuNum = 1; 
			}
			if (IntRect(318, 325, 518, 50).contains(Mouse::getPosition(window))) 
			{ 
				menuNum = 3; 
			}

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (menuNum == 1)
				{
					isMenu = false;//если нажали первую кнопку, то выходим из меню 
				}
				if (menuNum == 3)
				{ 
					window.close(); 
					isMenu = false;
				}
			}
			window.draw(menu1);
			window.draw(menu2);
			window.display();
		}
	}
}

class Player
{
public:

	float dx, dy;
	Sprite s;
	FloatRect rect;
	int ShipX = 68;
	int ShipY = 66;
	int ShipPosX = DisplayX - ShipX;
	int ShipPosY = DisplayY - ShipY;
	Player(Texture& image)
	{
		s.setTexture(image);
		rect = FloatRect(ShipPosX / 2, DisplayY, ShipX, ShipY);
		s.setPosition(0, 0);
		dx = dy = 0;
	}

	void update(float time)
	{
		rect.left += dx * time;
		rect.top += dy * time;
		if (rect.left > ShipPosX)
			rect.left = ShipPosX;
		if (rect.left < 0)
			rect.left = 0;
		if (rect.top > ShipPosY)
			rect.top = ShipPosY;
		if (rect.top < 0)
			rect.top = 0;

		s.setPosition(rect.left, rect.top);

		dx = 0;
		dy = 0;
	}
};

class Bullet
{
public:
	float dx, dy,x,y,w,h;
	bool life;
	Sprite sprite;

	FloatRect rect;
	int BulletX = 34;
	int BulletY = 46;
	Bullet(Texture& image,int X,int Y, bool Life)
	{
		sprite.setTexture(image);
		rect = FloatRect(X+20,Y, BulletX, BulletY);
		sprite.setPosition(X, Y);
		dx = dy = 150;
		life = Life;
	}
	void update(float time)
	{ 
		if (rect.top < DisplayY)
		{
			rect.top -= dy * time;
			sprite.setPosition(rect.left, rect.top);
		}
		 
		if (sprite.getPosition().y < -100)
		{
			life = false;
		}
	}
};

class Enemy
{
public:
	float dx, dy, x, y, w, h;
	bool life;
	Sprite sprite;
	bool right;
	FloatRect rect;
	int t;
	Enemy(Texture& image, int X, int Y, int t)
	{
		sprite.setTexture(image);
		rect = FloatRect(X, Y, EnemyX, EnemyY);
		sprite.setPosition(X, Y);
		dx = dy = 50+t;
		life = true;
	    right = true;

	}
	void update(float time)
	{
		if (right)
		{
			rect.left += dx * time;
			sprite.setPosition(rect.left, rect.top); 
			if (sprite.getPosition().x > EnemyPosX-10)
				right = false;
		}
		else
		{
			rect.left -= dx * time;
			sprite.setPosition(rect.left, rect.top);
			if (sprite.getPosition().x < 10)
				right = true;
		}
	/*	if (rect.top < DisplayY)
		{
			rect.top -= dy * time;
			sprite.setPosition(rect.left, rect.top);
		} */
	}
};

class Eggs
{
public:
	float dx, dy, x, y, w, h;
	bool life;
	Sprite sprite;

	FloatRect rect;
	int EggsX = 35;
	int EggsY = 45;
	Eggs(Texture& image, int X, int Y, bool Life)
	{
		sprite.setTexture(image);
		rect = FloatRect(X + 20, Y, EggsX, EggsY);
		sprite.setPosition(X, Y);
		dx = dy = 150;
		life = Life;
	}
	void update(float time)
	{
		rect.top = dy * time;
		sprite.setPosition(rect.left, rect.top);

		if (sprite.getPosition().y > 700)
		{
			life = false;
		}
	}
};

void GAME(RenderWindow& window)
{
	int score = 0;
	int level = 1;
	String s = "Score: 000";
	bool countScore = false;
	bool levelCreated = true;
	Font font;//Ўрифт
	font.loadFromFile("Font//Miratrix-Normal.otf");
	Text text("", font, 20);
	text.setFillColor(Color::Blue);

	Texture t, b, k, e;
	t.loadFromFile("images//hero.png");//главный герой
	Player p(t);
	b.loadFromFile("images//bullet.png");//выстрел
	k.loadFromFile("images//kura.png");//курица
	e.loadFromFile("images//eggs.png");//€йца
	//Enemy e(k);

	//Bullet fire(b);
	std::vector<Bullet> myBullets;
	std::vector<Bullet> enemyEggs;
	std::vector<Enemy> myEnemys;
	Clock clock;

	while (window.isOpen())
	{
		if (levelCreated)
		{
			if (level == 1)
			{
				myEnemys.push_back(Enemy(k, EnemyPosX / 2, 50, 0));
			}
			if (level == 3)
			{
				myEnemys.push_back(Enemy(k, EnemyX, 50, 50));
				myEnemys.push_back(Enemy(k, EnemyPosX / 2, 100, 50));
				myEnemys.push_back(Enemy(k, EnemyPosX - EnemyX, 150, 50));
			}
			if (level == 200)
			{
				myEnemys.push_back(Enemy(k, EnemyX, 50, 100));
				myEnemys.push_back(Enemy(k, EnemyPosX / 2, 100, 100));
				myEnemys.push_back(Enemy(k, EnemyPosX - EnemyX, 150, 100));
				myEnemys.push_back(Enemy(k, EnemyX, 200, 100));
				myEnemys.push_back(Enemy(k, EnemyPosX / 3, 250, 100));
				myEnemys.push_back(Enemy(k, EnemyPosX / 4, 150, 100));
			}
			//level=level+1;
			//cout << level << endl;
			levelCreated = false;
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		time = time * 2;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
				{
					myBullets.push_back(Bullet(b, p.s.getPosition().x - 2, p.s.getPosition().y - 30, true));
				}
		}
		//myBullets.push_back(Bullet(e, k.s.getPosition().x - 2, k.s.getPosition().y - 30, true));
		if ((Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::A)))
		{
			p.dx = -100;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::D)))
		{
			p.dx = 100;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::W)))
		{
			p.dy = -100;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Down)) || (Keyboard::isKeyPressed(Keyboard::S)))
		{
			p.dy = 100;
		}

		if (Keyboard::isKeyPressed(Keyboard::T))
		{
			level++;
			levelCreated = true;
		}

		p.update(time);

		window.clear();
		window.draw(p.s);

		if (!enemyEggs.empty())
		{
			for (int i = 0; i < enemyEggs.size(); i++)
			{
				if (enemyEggs[i].life == false)
				{
					enemyEggs.erase(enemyEggs.begin() + i);
				}
			}
		}

		if (!enemyEggs.empty())
		{
			for (int i = 0; i < enemyEggs.size(); i++)
			{
				enemyEggs[i].update(time);
				window.draw(enemyEggs[i].sprite);

			}
		}

		if (!myEnemys.empty())
		{
			for (int i = 0; i < myEnemys.size(); i++)
			{
				if (myEnemys[i].life == false)
				{
					myEnemys.erase(myEnemys.begin() + i);

				}
			}
		}

		if (!myEnemys.empty())
		{
			for (int i = 0; i < myEnemys.size(); i++)
			{
				myEnemys[i].update(time);
				window.draw(myEnemys[i].sprite);

			}
		}

		if (!myBullets.empty())
		{
			for (int i = 0; i < myBullets.size(); i++)
			{
				if (myBullets[i].life == false)
				{
					myBullets.erase(myBullets.begin() + i);
				}
			}
		}


		if (!myBullets.empty())
		{
			for (int i = 0; i < myBullets.size(); i++)
			{
				myBullets[i].update(time);
				window.draw(myBullets[i].sprite);
			}
		}

		if (!myEnemys.empty() && !myBullets.empty())
		{
			for (int i = 0; i < myEnemys.size(); i++)
			{
				for (int j = 0; j < myBullets.size(); j++)
				{
					if ((myBullets[j].sprite.getPosition().y > myEnemys[i].sprite.getPosition().y) && (myBullets[j].sprite.getPosition().y < myEnemys[i].sprite.getPosition().y + EnemyY)
						&& ((myBullets[j].sprite.getPosition().x > myEnemys[i].sprite.getPosition().x) && (myBullets[j].sprite.getPosition().x < myEnemys[i].sprite.getPosition().x + EnemyX)))
					{
						myBullets[j].life = false;
						myEnemys[i].life = false;
						countScore = true;
					}
				}
			}
		}

		if (countScore)
		{
			score = score + 100 * level;
			s = "Score: " + std::to_string(score);
			countScore = false;
		}
		text.setString(s);
		text.setPosition(0, 0);
		window.draw(text);
		window.display();
	}
}

int main()
{ 
	RenderWindow window(VideoMode(DisplayX, DisplayY), "You or chicken");
	menu(window);//вызов меню
	GAME(window);
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		return 0; //если эскейп, то выходим из игры
	}
	return 0;
}