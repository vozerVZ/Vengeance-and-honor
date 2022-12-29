#include "menu.h"

void menu(RenderWindow& window) {
	// Load textures
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground, warriorTexture, skeletonTexture, cursorTexture;
	menuTexture1.loadFromFile("images/menu/1.png");
	menuTexture2.loadFromFile("images/menu/2.png");
	menuTexture3.loadFromFile("images/menu/3.png");
	aboutTexture.loadFromFile("images/menu/about.png");
	menuBackground.loadFromFile("images/menu/castle.jpg");
	warriorTexture.loadFromFile("images/menu/warrior.png");
	skeletonTexture.loadFromFile("images/menu/skeleton.png");
	cursorTexture.loadFromFile("images/menu/cursor.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground), warrior(warriorTexture), skeleton(skeletonTexture), cursor(cursorTexture);
	bool isMenu = 1;
	bool isCollision = false;
	bool isAboutDrawing = false;
	bool isGameStarting = false;
	int menuNum = 0;
	float menuAnimFrame = 0;
	// Set textures
	menu1.setPosition(0, 120);
	warrior.setPosition(170, 110);
	menu2.setPosition(0, 180);
	menu3.setPosition(0, 240);
	skeleton.setPosition(100, 230);
	menuBg.setPosition(0, 0);

	cursor.setScale(0.1, 0.1);

	RectangleShape fade_end(Vector2f(800, 600));
	fade_end.setFillColor(Color(0, 0, 0, 0));
	fade_end.setPosition(0, 0);

	Clock menu_clock;

	menuNum = 0;

	float bgAlpha = 0;

	while (isMenu) {
		float menu_time = menu_clock.getElapsedTime().asMicroseconds();
		menu_clock.restart();
		menu_time = menu_time / 800;
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		
		menuNum = 0;

		window.clear();
		// Set textures of warrior/skeleton near buttons
		warrior.setTextureRect(IntRect(0, 0, 48, 48));
		skeleton.setTextureRect(IntRect(0, 0, 48, 48));

		if (!isCollision) { menuAnimFrame = 0; }
		isCollision = false;

		cursor.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

		if (isGameStarting) {
			bgAlpha += 0.3;
			if (int(bgAlpha > 255)) {
				isMenu = false;
			}
		}

		// Start animation when cursor collide with button
		if (IntRect(0, 120, 180, 50).contains(Mouse::getPosition(window))) {
			isCollision = true;
			menu1.setColor(Color::Blue);
			menuNum = 1;
			menuAnimFrame += 0.005 * menu_time;
			warrior.setTextureRect(IntRect(48 * int(menuAnimFrame), 0, 48, 48));
			if (menuAnimFrame > 6) menuAnimFrame = 0;
		}
		if (IntRect(0, 180, 200, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(0, 240, 120, 50).contains(Mouse::getPosition(window))) {
			isCollision = true;
			menu3.setColor(Color::Blue);
			menuNum = 3;
			menuAnimFrame += 0.005 * menu_time;
			skeleton.setTextureRect(IntRect(48 * int(menuAnimFrame), 0, 48, 48));
			if (menuAnimFrame > 8) menuAnimFrame = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape) && isAboutDrawing) {
			isAboutDrawing = false;
		}

		// Click on buttons
		if (Mouse::isButtonPressed(Mouse::Left) && !isAboutDrawing) {
			if (menuNum == 1) { isGameStarting = true; }
			if (menuNum == 2) { isAboutDrawing = true; }
			if (menuNum == 3) { window.close(); isMenu = false; }
		}

		fade_end.setFillColor(Color(0, 0, 0, int(bgAlpha)));

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(warrior);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(skeleton);
		window.draw(cursor);

		if (isAboutDrawing) {
			window.draw(about);
		}

		if (isGameStarting) {
			window.draw(fade_end);
		}

		window.display();
	}

}