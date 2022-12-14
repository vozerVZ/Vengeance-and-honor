#include "map.h"
#include "player.h"
#include "enemy.h"
#include "menu.h"
#include <vector>
#include <sstream>
using namespace sf;

int main(){
	RenderWindow window(sf::VideoMode(800, 600), "Vengeance and honor(indev)");
	menu(window);
	View view;
	view.reset(sf::FloatRect(0, 0, 800, 600));
	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	Clock clock;

	Font font;
	font.loadFromFile("fonts/CyrilicOld.ttf");
	Text text("", font, 13);
	text.setFillColor(Color::Red);
	text.setStyle(Text::Bold);

	Player p("hero.png", 200, 200, 96.0, 96.0);

	bool debugMode = true;
	int debugButtonDelay = 0;

	std::vector<Enemy*> enemies;
	//MAP CREATING-------------------------------------------------------------
	srand((unsigned)time(0));
	const int MAX_LEAF_SIZE = 300;
	vector<Leaf*> leafs;

	Leaf root(0, 0, 1400, 900);
	leafs.push_back(&root);

	bool splitCycle = true;

	while (splitCycle) {
		splitCycle = false;

		for (int i = 0; i < leafs.size(); i++) {
			if (leafs[i]->isRightChildNull && leafs[i]->isLeftChildNull) {
				if (leafs[i]->width > MAX_LEAF_SIZE || leafs[i]->height > MAX_LEAF_SIZE || (rand() % 100) > 25) {
					if (leafs[i]->split()) {
						leafs.push_back(leafs[i]->leftChild);
						leafs.push_back(leafs[i]->rightChild);
						splitCycle = true;
					}
				}
			}
		}
	}

	root.createRoom();

	int tileMap[90][140];
	memset(tileMap, 0, sizeof(tileMap));

	for (auto l : leafs) { // Transfer from classes to map
		if (6 < l->width < 20 && 6 < l->height < 20) {
			if (l->isRoomCreate) {
				for (int i = l->room[0] / 10; i < (l->room[0] + l->room[2]) / 10 + 1; i++) {
					for (int j = l->room[1] / 10; j < (l->room[1] + l->room[3]) / 10 + 1; j++) {
						tileMap[j][i] = 1;
					}
				}

				enemies.push_back(new Enemy("enemy.png", 96.0, 96.0, l->room[0] / 10 * 32, l->room[1] / 10 * 32, ((l->room[0] + l->room[2]) / 10 + 1) * 32, ((l->room[1] + l->room[3]) / 10 + 1)* 32, font));
				enemies.push_back(new Enemy("enemy.png", 96.0, 96.0, l->room[0] / 10 * 32, l->room[1] / 10 * 32, ((l->room[0] + l->room[2]) / 10 + 1) * 32, ((l->room[1] + l->room[3]) / 10 + 1)* 32, font));
				enemies.push_back(new Enemy("enemy.png", 96.0, 96.0, l->room[0] / 10 * 32, l->room[1] / 10 * 32, ((l->room[0] + l->room[2]) / 10 + 1) * 32, ((l->room[1] + l->room[3]) / 10 + 1)* 32, font));
			}
			if (l->halls.size() > 0) {
				for (auto k : l->halls) {
					for (int i = k[0] / 10; i < (k[0] + k[2]) / 10 + 1; i++) {
						for (int j = k[1] / 10; j < (k[1] + k[3]) / 10 + 1; j++) {
							tileMap[j][i] = 1;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 90; i++) { // creating walls
		for (int j = 0; j < 140; j++) {
			if (tileMap[i][j] == 0) {
				if (i > 0 && tileMap[i - 1][j] == 1) { tileMap[i][j] = 2; }
				else if (i < 89 && tileMap[i + 1][j] == 1) { tileMap[i][j] = 2; }
				else if (j > 0 && tileMap[i][j - 1] == 1) { tileMap[i][j] = 2; }
				else if (j < 139 && tileMap[i][j + 1] == 1) { tileMap[i][j] = 2; }
				else if (i < 89 && j < 139 && tileMap[i + 1][j + 1] == 1) { tileMap[i][j] = 2; }
				else if (i > 0 && j < 139 && tileMap[i - 1][j + 1] == 1) { tileMap[i][j] = 2; }
				else if (i < 89 && j > 0 && tileMap[i + 1][j - 1] == 1) { tileMap[i][j] = 2; }
				else if (i > 0 && j > 0 && tileMap[i - 1][j - 1] == 1) { tileMap[i][j] = 2; }
			}
		}
	}

	/*for (int i = 0; i < 90; i++) {
		for (int j = 0; j < 140; j++) {
			cout << tileMap[i][j];
		}
		cout << endl;
	}
	*/
	
	//-------------------------------------------------------------------------------------------------------------
	while (window.isOpen()){
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//Keyboard control
		p.move(reinterpret_cast<int*>(tileMap), 90, 140, time);
		
		if(Keyboard::isKeyPressed(Keyboard::P) and debugButtonDelay == 0){
			debugMode = !debugMode;
			debugButtonDelay = 20;
		}
		if (debugButtonDelay > 0) { debugButtonDelay--; }

		if (Keyboard::isKeyPressed(Keyboard::Enter) && p.attackTimer == 0 && p.life) {
			p.attackTimer = 100;
			p.idleTimer = 0;
			p.animType = 2;
			p.CurrentFrame = 0;
			for (int i = 0; i < enemies.size(); i++) {
				if(enemies[i]->getRect().intersects(p.getRect()) && enemies[i]->life){
					enemies[i]->getDamage(p.damage);
					enemies[i]->mode = 1;
					if (!enemies[i]->isCount) { enemies[i]->isCount = true; p.attackersCount++; }
					break;
				}
			}	
		}

		p.update(time);
		
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->update(time, p.getplayercoordinateX(), p.getplayercoordinateY(), p);
		}

		view.setCenter(p.getplayercoordinateX(), p.getplayercoordinateY());
		window.setView(view);
		window.clear();

		//Map drawing
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 140; j++){
				if (tileMap[i][j] == 0)  s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if (tileMap[i][j] == 1)  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (tileMap[i][j] == 2)  s_map.setTextureRect(IntRect(64, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		}

		p.draw(window, debugMode);

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->draw(window, debugMode);
			if (enemies[i]->life) {
				std::ostringstream enemyLVL;
				enemyLVL << enemies[i]->level;
				text.setFillColor(Color(0, 0, 0));
				text.setString(enemies[i]->name + " " + enemyLVL.str() + " lvl");
				text.setPosition(enemies[i]->sprite.getPosition().x, enemies[i]->sprite.getPosition().y);
				window.draw(text);
			}
		}

		std::ostringstream playerHP;
		playerHP << p.health;
		std::ostringstream playerMaxHP;
		playerMaxHP << p.maxHealth;
		text.setFillColor(Color(255, 0, 0));
		text.setString(playerHP.str() + '/' + playerMaxHP.str());
		text.setPosition(view.getCenter().x - 248, view.getCenter().y - 301);
		window.draw(text);

		window.display();
	}
	return 0;
}