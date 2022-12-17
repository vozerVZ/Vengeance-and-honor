#include "Game.h"

Game::Game() {
	debugMode = true;
	debugButtonDelay = 0;
	memset(tileMap, 0, sizeof(tileMap));
}

Game::~Game() {
}

int Game::run() {
	// Prepare everything
	RenderWindow window(sf::VideoMode(800, 600), "Vengeance and honor(indev)");
	view.reset(sf::FloatRect(0, 0, 800, 600));
	map_image.loadFromFile("images/map.png");
	map.loadFromImage(map_image);
	s_map.setTexture(map);
	font.loadFromFile("fonts/CyrilicOld.ttf");
	Text text("", font, 13);
	text.setFillColor(Color::Red);
	text.setStyle(Text::Bold);

	Player p("hero.png", 200, 200, 96.0, 96.0);
	Leaf root(0, 0, 1400, 900);
	leafs.push_back(&root);

	// Generating map
	createMap(enemies, font);

	srand((unsigned)time(0));

	menu(window);

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//Keyboard control
		p.move(reinterpret_cast<int*>(tileMap), 90, 140, time);

		if (Keyboard::isKeyPressed(Keyboard::P) and debugButtonDelay == 0) {
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
				if (enemies[i]->getRect().intersects(p.getRect()) && enemies[i]->life) {
					enemies[i]->getDamage(p.damage);
					enemies[i]->mode = 1;
					if (!enemies[i]->isCount) { enemies[i]->isCount = true; p.attackersCount++; }
					break;
				}
			}
		}
		// Player and enemies update/draw
		p.update(time);

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->update(time, p.getplayercoordinateX(), p.getplayercoordinateY(), p);
		}

		view.setCenter(p.getplayercoordinateX(), p.getplayercoordinateY());
		window.setView(view);
		window.clear();

		// Map drawing
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 140; j++) {
				if (tileMap[i][j] == 0)  s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if (tileMap[i][j] == 1)  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (tileMap[i][j] == 2)  s_map.setTextureRect(IntRect(64, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		}

		p.draw(window, debugMode);
		// Drawing name + level of enemies above them(bc from class drawing text causing "Acces reading violation"). Try to fix?
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
		// Draw player healthbar(same problem)
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
