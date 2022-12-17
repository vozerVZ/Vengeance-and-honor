#include "menu.h"

void menu(RenderWindow& window) {
	// Load textures
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground, warriorTexture, skeletonTexture;
	menuTexture1.loadFromFile("images/menu/1.png");
	menuTexture2.loadFromFile("images/menu/2.png");
	menuTexture3.loadFromFile("images/menu/3.png");
	aboutTexture.loadFromFile("images/menu/about.png");
	menuBackground.loadFromFile("images/menu/castle.jpg");
	warriorTexture.loadFromFile("images/menu/warrior.png");
	skeletonTexture.loadFromFile("images/menu/skeleton.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground), warrior(warriorTexture), skeleton(skeletonTexture);
	bool isMenu = 1;
	bool isCollision = false;
	int menuNum = 0;
	float menuAnimFrame = 0;
	// Set textures
	menu1.setPosition(0, 120);
	warrior.setPosition(170, 110);
	menu2.setPosition(0, 180);
	menu3.setPosition(0, 240);
	skeleton.setPosition(100, 230);
	menuBg.setPosition(0, 0);

	Clock menu_clock;

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
		// Click on buttons
		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }
		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(warrior);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(skeleton);

		window.display();
	}

}