#include "Game.h"

Game::Game() {
	debugMode = true;
	debugButtonDelay = 0;
	memset(tileMap, 0, sizeof(tileMap));
	// Cards enum 
	cards.push_back("Three of diamonds");
	cards.push_back("Three of hearts");
	cards.push_back("Three of clubs");
	cards.push_back("Three of spades");

	cards.push_back("Four of diamonds");
	cards.push_back("Four of hearts");
	cards.push_back("Four of clubs");
	cards.push_back("Four of spades");

	cards.push_back("Five of diamonds");
	cards.push_back("Five of hearts");
	cards.push_back("Five of clubs");
	cards.push_back("Five of spades");

	cards.push_back("Six of diamonds");
	cards.push_back("Six of hearts");
	cards.push_back("Six of clubs");
	cards.push_back("Six of spades");

	table1 = 0; // 0
	table2 = 0; // 15
	table3 = 0; // 5
	table4 = 0; // 10
}

Game::~Game() {
}

int Game::run() {
	// Prepare everything
	RenderWindow window(sf::VideoMode(800, 600), "Vengeance and honor(indev)");
	view.reset(sf::FloatRect(0, 0, 800, 600));
	map_image.loadFromFile("images/tilesets/map.png");
	map.loadFromImage(map_image);
	s_map.setTexture(map);
	font.loadFromFile("fonts/Hamlin-Regular.ttf");
	Text text("", font, 13);
	text.setFillColor(Color::White);
	text.setStyle(Text::Bold);
	//Sprite of end picture
	Texture startImg;
	startImg.loadFromFile("images/story/start.png");
	Sprite start(startImg);

	Player p("tilesets/hero.png", 0, 0, 96.0, 96.0);

	// Starting leaf class to dividing
	Leaf root(0, 0, 1400, 900);
	leafs.push_back(&root);

	// Black rectangle for writing info
	RectangleShape r(Vector2f(800.0f, 100.0f));
	r.setPosition(view.getCenter().x - 400, view.getCenter().y + 200);
	r.setFillColor(Color::Black);
	r.setOutlineColor(Color::Red);
	r.setOutlineThickness(2);

	// Fade after boss eath
	float endAlpha = 255.0f;
	RectangleShape fade(Vector2f(800, 600));
	fade.setFillColor(Color(0, 0, 0, 255));
	fade.setPosition(0, 0);

	// Tile for drawing minimap
	RectangleShape minimapTile(Vector2f(6, 7));
	minimapTile.setFillColor(Color(0, 0, 0));
	minimapTile.setPosition(0, 0);

	// Bias of minimap
	int xMapBias = 0;
	int yMapBias = 0;
	bool isMapDrawing = false;

	// Generating map
	srand((unsigned)time(0));

	createMap(enemies, font);

	window.setMouseCursorVisible(false);

	menu(window);

	// setting coordinates of minimum-coordinate room
	p.setplayercoordinateX(xMin * 32);
	p.setplayercoordinateY(yMin * 32);

	Enemy boss("tilesets/boss.png", 1, 96.0, 96.0, bossRoomX, bossRoomY, bossRoomW, bossRoomH, font);

	enemies.push_back(&boss);

	bool gameFlag = true;
	bool isGameClosedWithESC = false;

	while(gameFlag){
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
				window.close();
				gameFlag = false;
				isGameClosedWithESC = true;
			}
		}

		if (!isMapDrawing) {
			p.move(reinterpret_cast<int*>(tileMap), 90, 140, time, enemies);
		}

		//Keyboard control
		if (Keyboard::isKeyPressed(Keyboard::P) and debugButtonDelay == 0) {
			debugMode = !debugMode;
			debugButtonDelay = 20;
		}
		if (debugButtonDelay > 0) { debugButtonDelay--; }

		if(Keyboard::isKeyPressed(Keyboard::E) && debugButtonDelay == 0){
			// Wells
			if (!p.isWellRiddleSolved) {
				if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					p.copperCoins = 15; p.silverCoins = 3; p.goldCoins = 0;
				}
				else if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32 - 2) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					if (p.copperCoins >= 2) { p.copperCoins -= 2; }
				}
				else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 3) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					if (p.copperCoins >= 1) { p.copperCoins -= 1; p.silverCoins += 1; }
				}
				else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 5) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					if (p.copperCoins >= 1 && p.silverCoins >= 1) { p.copperCoins -= 1; p.silverCoins -= 1; p.goldCoins += 1; }
				}
				else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 7) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					if (p.goldCoins >= 1 && p.silverCoins >= 1) { p.goldCoins -= 1; p.silverCoins -= 1; p.copperCoins += 1; }
				}
			}
			// Tables
			if (!p.isTableRiddleSolved) {
				if (xTable == int((p.getplayercoordinateX() + p.w / 2) / 32 - 2) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					table1 += 1;
					if (table1 > 15) table1 = 0;
				}
				else if (xTable == int((p.getplayercoordinateX() + p.w / 4) / 32 - 3) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					table2 += 1;
					if (table2 > 15) table2 = 0;
				}
				else if (xTable == int((p.getplayercoordinateX() + p.w / 4) / 32 - 5) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					table3 += 1;
					if (table3 > 15) table3 = 0;
				}
				else if (xTable == int((p.getplayercoordinateX() + p.w / 4) / 32 - 7) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
					table4 += 1;
					if (table4 > 15) table4 = 0;
				}
			}
			debugButtonDelay = 30;
		}
		// Cheats
		if (Keyboard::isKeyPressed(Keyboard::Num1)) { p.setplayercoordinateX(xWell * 32 + 100); p.setplayercoordinateY(yWell * 32 + 30); } // Teleport to wells riddle
		if (Keyboard::isKeyPressed(Keyboard::Num2)) { p.setplayercoordinateX(xTable * 32 + 100); p.setplayercoordinateY(yTable * 32 + 30); } // Teleport to cards riddle
		if (Keyboard::isKeyPressed(Keyboard::Num3)) { p.setplayercoordinateX(boss.getenemycoordinateX()); p.setplayercoordinateY(boss.getenemycoordinateY()); } // Teleport to boss
		if (Keyboard::isKeyPressed(Keyboard::Num4)) { p.copperCoins = 0; p.silverCoins = 0; p.goldCoins = 0; } // Solve wells riddle
		if (Keyboard::isKeyPressed(Keyboard::Num5)) { table1 = 0; table2 = 15; table3 = 5; table4 = 10; } // Solve cards riddle
		if (Keyboard::isKeyPressed(Keyboard::Num6)) { p.level = 20; p.maxHealth = 1000; p.health = 1000; p.damage = 500; } // Buff
		//Control minimap
		if (Keyboard::isKeyPressed(Keyboard::Left)) { xMapBias += 1; }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { xMapBias -= 1; }
		if (Keyboard::isKeyPressed(Keyboard::Up)) { yMapBias += 1; }
		if (Keyboard::isKeyPressed(Keyboard::Down)) { yMapBias -= 1; }
		if (Keyboard::isKeyPressed(Keyboard::M) && debugButtonDelay == 0) {
			isMapDrawing = !isMapDrawing; 
			debugButtonDelay = 10; 
			xMapBias = 0; yMapBias = 0; 
		}

		// Player and enemies update/draw
		p.update(time);

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->update(time, p.getplayercoordinateX(), p.getplayercoordinateY(), p.w, p.h, p.health, p.attackersCount);
		}

		// Clearing the screen and move camera to player
		view.setCenter(p.getplayercoordinateX(), p.getplayercoordinateY());
		window.setView(view);
		window.clear();
		// Clear wells after solving
		if (p.isWellRiddleSolved && tileMap[yWell][xWell + 2] == 4) {
			tileMap[yWell][xWell + 2] = 1;
			tileMap[yWell][xWell + 4] = 1;
			tileMap[yWell][xWell + 6] = 1;
			tileMap[yWell][xWell + 8] = 1;
		}
		// Check if table riddle solved
		if (!p.isTableRiddleSolved && table1 == 0 && table2 == 15 && table3 == 5 && table4 == 10) {
			p.isTableRiddleSolved = true;
		}

		// Map drawing
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 140; j++) {
				if (tileMap[i][j] == 0)  s_map.setTextureRect(IntRect(96, 0, 32, 32)); // black square(out-of-bounds)
				if (tileMap[i][j] == 1)  s_map.setTextureRect(IntRect(0, 0, 32, 32)); // wooden floor
				if (tileMap[i][j] == 2)  s_map.setTextureRect(IntRect(64, 0, 32, 32)); // cobble wal
				if (tileMap[i][j] == 4)  s_map.setTextureRect(IntRect(128, 0, 32, 32)); // well
				if (tileMap[i][j] == 5)  s_map.setTextureRect(IntRect(160, 0, 32, 32)); // cards table
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

		// Draw player hp(same problem as text of enemy's info)
		std::ostringstream playerHP;
		playerHP << p.health;
		std::ostringstream playerMaxHP;
		playerMaxHP << p.maxHealth;
		text.setFillColor(Color(255, 0, 0));
		text.setString(playerHP.str() + '/' + playerMaxHP.str());
		text.setPosition(view.getCenter().x - 248, view.getCenter().y - 301);
		window.draw(text);
		// Level
		std::ostringstream playerLVL;
		playerLVL << p.level;
		text.setFillColor(Color(128, 128, 128));
		text.setString(playerLVL.str() + " level");
		text.setPosition(view.getCenter().x - 400, view.getCenter().y - 275);
		window.draw(text);
		// Coins
		if (abs(xWell - int((p.getplayercoordinateX() + p.w / 2) / 32)) <= 8 && abs(yWell - int((p.getplayercoordinateY() + p.h - 15) / 32)) < 3) {
			std::ostringstream money;
			money << p.copperCoins;
			text.setFillColor(Color(205, 127, 50));
			text.setString(money.str() + " copper coins");
			text.setPosition(view.getCenter().x - 400, view.getCenter().y - 260);
			window.draw(text);

			money.str("");
			money << p.silverCoins;
			text.setFillColor(Color(192, 192, 192));
			text.setString(money.str() + " silver coins");
			text.setPosition(view.getCenter().x - 400, view.getCenter().y - 245);
			window.draw(text);

			money.str("");
			money << p.goldCoins;
			text.setFillColor(Color(255, 215, 0));
			text.setString(money.str() + " golden coins");
			text.setPosition(view.getCenter().x - 400, view.getCenter().y - 230);
			window.draw(text);
		}

		// Wells riddle text
		r.setPosition(view.getCenter().x - 400, view.getCenter().y + 200);
		text.setFillColor(Color(255, 0, 0));
		if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			if (!p.isWellRiddleSolved) {
				text.setString("It's wells of cursed moneys. You have 15 copper coins, 3 silver and 0 gold coins. \nYou must get rid of them. This well reset your coins.");
			}else{
				text.setString("You are only the monster");
			}
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 2) / 32 - 2) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32) && !p.isWellRiddleSolved) {
			text.setString("This well takes two copper coins from you.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 3) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32) && !p.isWellRiddleSolved) {
			text.setString("This well takes one copper coin and give you one silver.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 5) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32) && !p.isWellRiddleSolved) {
			text.setString("This well takes one copper coin and one silver coin and give you one gold coin.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}else if (xWell == int((p.getplayercoordinateX() + p.w / 4) / 32 - 7) && yWell == int((p.getplayercoordinateY() + p.h - 15) / 32) && !p.isWellRiddleSolved) {
			text.setString("This well takes one silver coin and one gold coin and give you one copper coin.");
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}

		//Tables riddle text
		if (xTable == int((p.getplayercoordinateX() + p.w / 2) / 32) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			if (!p.isTableRiddleSolved) {
				window.draw(r);
				text.setPosition(view.getCenter().x - 400 + 20, view.getCenter().y + 200 + 10);
				text.setString("The cards on either side of the four are black. The club is to the right of the three, but not next to it.");
				window.draw(text);

				text.setString("The spade is located to the left of the heart.");
				text.setPosition(view.getCenter().x - 400 + 20, view.getCenter().y + 200 + 30);
				window.draw(text);

				text.setString("The two middle cards add up to an even number. None of them are clubs.");
				text.setPosition(view.getCenter().x - 400 + 20, view.getCenter().y + 200 + 50);
				window.draw(text);
			} else {
				text.setString("Why did you kill her?");
				window.draw(text);
			}
			
		} else if (xTable == int((p.getplayercoordinateX() + p.w / 2) / 32 - 2) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setString(cards[table1]);
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		} else if (xTable == int((p.getplayercoordinateX() + p.w / 4) / 32 - 3) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setString(cards[table2]);
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		} else if (xTable == int((p.getplayercoordinateX() + p.w / 4) / 32 - 5) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setString(cards[table3]);
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		} else if (xTable == int((p.getplayercoordinateX() + p.w / 4) / 32 - 7) && yTable == int((p.getplayercoordinateY() + p.h - 15) / 32)) {
			text.setString(cards[table4]);
			text.setPosition(view.getCenter().x - 400 + 50, view.getCenter().y + 200 + 50);
			window.draw(r);
			window.draw(text);
		}

		// Map drawing
		if (isMapDrawing) {
			window.clear();

			for (int i = 0; i < 90; i++) {
				for (int j = 0; j < 140; j++) {
					if (tileMap[i][j] == 0)  minimapTile.setFillColor(Color(0, 0, 0)); // black square(out-of-bounds)
					if (tileMap[i][j] == 1)  minimapTile.setFillColor(Color(150, 75, 0)); // wooden floor
					if (tileMap[i][j] == 2)  minimapTile.setFillColor(Color(128, 128, 128)); // cobble wal
					if (tileMap[i][j] == 4)  minimapTile.setFillColor(Color(50, 51, 50)); // well
					if (tileMap[i][j] == 5)  minimapTile.setFillColor(Color(0, 77, 0)); // cards table
					minimapTile.setPosition(view.getCenter().x - 400 + (j - 30 + xMapBias) * 6, view.getCenter().y - 300 + (i - 30 + yMapBias) * 7);
					window.draw(minimapTile);
				}
			}
		}
		// If boss is dead - exit the main game cycle
		if (!boss.life) { gameFlag = false; }

		window.display();
	}
	if (!isGameClosedWithESC) {
		// Resetting the view
		start.setPosition(0, 0);
		view.setCenter(400, 300);
		window.setView(view);
		// Drawing final page and fade
		while (true) {
			window.clear();

			window.draw(start);

			fade.setFillColor(Color(0, 0, 0, int(endAlpha)));
			endAlpha -= 0.02f;

			window.draw(fade);

			if (endAlpha < 0) { break; }

			window.display();
		}
	}

	return 0;
}
