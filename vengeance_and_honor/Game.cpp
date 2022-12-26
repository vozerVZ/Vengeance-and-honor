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
	font.loadFromFile("fonts/Hamlin-Regular.ttf");
	Text text("", font, 13);
	text.setFillColor(Color::Red);
	text.setStyle(Text::Bold);

	Player p("hero.png", 200, 200, 96.0, 96.0);
	Leaf root(0, 0, 1400, 900);
	leafs.push_back(&root);

	RectangleShape r(Vector2f(800.0f, 100.0f));
	r.setPosition(view.getCenter().x - 400, view.getCenter().y + 200);
	r.setFillColor(Color::Black);

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

		p.move(reinterpret_cast<int*>(tileMap), 90, 140, time, enemies);

		//Keyboard control
		if (Keyboard::isKeyPressed(Keyboard::P) and debugButtonDelay == 0) {
			debugMode = !debugMode;
			debugButtonDelay = 20;
		}
		if (debugButtonDelay > 0) { debugButtonDelay--; }

		if(Keyboard::isKeyPressed(Keyboard::E) && debugButtonDelay == 0 && !p.isWellRiddleSolved){
			if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
				p.copperCoins = 15; p.silverCoins = 3; p.goldCoins = 0;
			}else if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32 - 2) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
				if(p.copperCoins >= 2){ p.copperCoins -= 2; }
			}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 3) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
				if (p.copperCoins >= 1) { p.copperCoins -= 1; p.silverCoins += 1; }
			}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 5) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
				if (p.copperCoins >= 1 && p.silverCoins >= 1) { p.copperCoins -= 1; p.silverCoins -= 1; p.goldCoins += 1;}
			}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 7) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {	
				if (p.goldCoins >= 1 && p.silverCoins >= 1) { p.goldCoins -= 1; p.silverCoins -= 1; p.copperCoins += 1; }
			}
			debugButtonDelay = 50;
		}

		if (Keyboard::isKeyPressed(Keyboard::Num1)) { p.setplayercoordinateX(xWell * 32);p.setplayercoordinateX(xWell * 32 + 100); p.setplayercoordinateY(yWell * 32 + 30); }

		// Player and enemies update/draw
		p.update(time);

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->update(time, p.getplayercoordinateX(), p.getplayercoordinateY(), p.w, p.h, p.health, p.attackersCount);
		}

		view.setCenter(p.getplayercoordinateX(), p.getplayercoordinateY());
		window.setView(view);
		window.clear();

		// Map drawing
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 140; j++) {
				if (tileMap[i][j] == 0)  s_map.setTextureRect(IntRect(96, 0, 32, 32)); // black square(out-of-bounds)
				if (tileMap[i][j] == 1)  s_map.setTextureRect(IntRect(0, 0, 32, 32)); // wooden floor
				if (tileMap[i][j] == 2)  s_map.setTextureRect(IntRect(64, 0, 32, 32)); // cobble wal
				if (tileMap[i][j] == 3)  s_map.setTextureRect(IntRect(32, 0, 32, 32)); // boulder(test object of well-riddle)
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		}

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

		p.draw(window, debugMode);

		// Draw player healthbar(same problem as text of enemy's info)
		std::ostringstream playerHP;
		playerHP << p.health;
		std::ostringstream playerMaxHP;
		playerMaxHP << p.maxHealth;
		text.setFillColor(Color(255, 0, 0));
		text.setString(playerHP.str() + '/' + playerMaxHP.str());
		text.setPosition(view.getCenter().x - 248, view.getCenter().y - 301);
		window.draw(text);

		// Wells riddle
		//std::cout << xWell << " " << int((p.getplayercoordinateX() + p.w / 2) / 32) << std::endl;
		//std::cout << yWell << " " << int((p.getplayercoordinateY() + p.h / 2 - 15) / 32) << std::endl;
		r.setPosition(view.getCenter().x - 400, view.getCenter().y + 200);
		if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setFillColor(Color(255, 0, 0));
			text.setString("It's wells of cursed moneys. You have 15 copper coins, 3 silver and 0 gold coins. \nYou must get rid of them. This well reset your coins.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32 - 2) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setFillColor(Color(255, 0, 0));
			text.setString("This well takes two copper coins from you.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 3) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setFillColor(Color(255, 0, 0));
			text.setString("This well takes one copper coin and give you one silver.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 5) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setFillColor(Color(255, 0, 0));
			text.setString("This well takes one copper coin and one silver coin and give you one gold coin.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 7) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setFillColor(Color(255, 0, 0));
			text.setString("This well takes one silver coin and one gold coin and give you one copper coin.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}


		window.display();
	}
	return 0;
}
