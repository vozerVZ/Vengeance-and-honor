#include "player.h"

Player::Player(String F, float X, float Y, float W, float H) {
	dx = 0; dy = 0; speed = 0; dir = 0;
	maxHealth = 100;
	damage = 20;
	life = true;
	attackersCount = 0;
	attackTimer = 0;
	respawnTimer = 0;
	idleTimer = 0;
	healTimer = 0;
	health = maxHealth;
	File = F;
	w = W; h = H;
	x = X; y = Y;
	CurrentFrame = 0;
	// Texture load
	image.loadFromFile("images/" + File);
	image.createMaskFromColor(Color(41, 33, 59));
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, w, h));
	// Player hitbox
	hitBox.setSize(Vector2f(w, h));
	hitBox.setFillColor(Color(255, 0, 0, 0));
	hitBox.setOutlineColor(Color(0, 0, 255));
	hitBox.setPosition(x, y);
	hitBox.setOutlineThickness(1);
	// Player healthbars set
	maxHealthBar.setSize(Vector2f(150, 15));
	maxHealthBar.setFillColor(Color(128, 0, 0));
	maxHealthBar.setPosition(x - 400, y - 300);

	healthBar.setSize(Vector2f(150 * health / maxHealth, 15));
	healthBar.setFillColor(Color(255, 0, 0));
	healthBar.setPosition(x - 400, y - 300);
	//Leg hitbox set(red dot under legs)
	legHitBox.setRadius(3);
	legHitBox.setPosition(x + w / 2, y + h);
	legHitBox.setFillColor(Color(255, 0, 0));
}

void Player::update(float time) {
	// Timers decreasing
	if (attackTimer > 0) { attackTimer--; }
	if (respawnTimer > 0) { respawnTimer--; }
	if (healTimer > 0) { healTimer--; }
	// Moving
	switch (dir){
	case 0: dx = speed; dy = 0; break;
	case 1: dx = -speed; dy = 0; break;
	case 2: dx = 0; dy = speed; break;
	case 3: dx = 0; dy = -speed; break;
	}
	x += dx * time;
	y += dy * time;
	speed = 0;
	// Sprites/hitboxes/bars set
	sprite.setPosition(x, y);
	hitBox.setPosition(x, y);
	legHitBox.setPosition(x + w / 2, y + h - 15);

	maxHealthBar.setPosition(x - 400, y - 300);
	healthBar.setSize(sf::Vector2f(150 * health / maxHealth, 15));
	healthBar.setPosition(x - 400, y - 300);

	// Player heal
	if (healTimer == 0 && health < maxHealth && life && attackersCount == 0) {
		healTimer = 100;
		health += 5;
		if (health > maxHealth) {
			health = maxHealth;
		}
	}
	// Player respawn
	if (respawnTimer == 1) {
		life = true;
		health = maxHealth;
		CurrentFrame = 0;
		animType = 0;
		animDir = 0;
		sprite.setTextureRect(IntRect(0, 0, 96, 96));
	}
	// Death state and animation
	if (health <= 0) {
		life = false;
		attackersCount = 0;
		if (animType != 3) {
			animType = 3;
			CurrentFrame = 0;
			respawnTimer = 1000;
		}
		if (CurrentFrame <= ANIM_DEATH_MAX_FRAMES) {
			CurrentFrame += 0.005 * time;
			sprite.setTextureRect(IntRect(96 * (int(CurrentFrame) + 19), 96 * animDir, 96, 96));
			idleTimer = 0;
		}
	}
	// Idle animation
	if (idleTimer < 50) { idleTimer++; }
	if (idleTimer == 50 and life) { 
		if (animType != 1) {
			animType = 1;
			CurrentFrame = 0;
		}
		CurrentFrame += 0.005 * time;
		if (CurrentFrame > ANIM_IDLE_MAX_FRAMES) CurrentFrame -= ANIM_IDLE_MAX_FRAMES;
		sprite.setTextureRect(IntRect(96 * (int(CurrentFrame) + 8), 96 * animDir, 96, 96));
	}
	// Attack animation
	if (animType == 2) {
		idleTimer = 0;
		CurrentFrame += 0.005 * time;
		sprite.setTextureRect(IntRect(96 * (int(CurrentFrame) + 13), 96 * animDir, 96, 96));
		if (CurrentFrame > ANIM_ATTACK_MAX_FRAMES) {
			animType = 0;
			CurrentFrame = 0;
		}
	}
}

void Player::move(const int tileMap[], size_t m, size_t n, float t, std::vector<Enemy*>& en){
	if (life) {
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			if (tileMap[(int)(y + h - 15) / 32 * n + (int)(x + w / 2 - 3) / 32] != 2) {
				if (animType != 0) { CurrentFrame = 0; }
				animDir = 1; animType = 0; idleTimer = 0;
				dir = 1; speed = 0.1;
				CurrentFrame += 0.005 * t;
				if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
				sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96 * animDir, 96, 96));
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			if (tileMap[(int)(y + h - 15) / 32 * n + (int)(x + w / 2 + 3) / 32] != 2) {
				if (animType != 0) { CurrentFrame = 0; }
				animDir = 0; animType = 0; idleTimer = 0;
				dir = 0; speed = 0.1;
				CurrentFrame += 0.005 * t;
				if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
				sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96 * animDir, 96, 96));
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			if (tileMap[(int)(y + h - 3 - 15) / 32 * n + (int)(x + w / 2) / 32] != 2) {
				if (animType != 0) { CurrentFrame = 0; }
				animDir = 3; animType = 0; idleTimer = 0;
				dir = 3; speed = 0.1;
				CurrentFrame += 0.005 * t;
				if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
				sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96 * animDir, 96, 96));
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			if (tileMap[(int)(y + h + 3 - 15) / 32 * n + (int)(x + w / 2) / 32] != 2) {
				if (animType != 0) { CurrentFrame = 0; }
				animDir = 2; animType = 0; idleTimer = 0;
				dir = 2; speed = 0.1;
				CurrentFrame += 0.005 * t;
				if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
				sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96 * animDir, 96, 96));
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter) && attackTimer == 0 && life) {
			attackTimer = 100;
			idleTimer = 0;
			animType = 2;
			CurrentFrame = 0;
			for (int i = 0; i < en.size(); i++) {
				if (en[i]->getRect().intersects(getRect()) && en[i]->life) {
					en[i]->getDamage(damage);
					en[i]->mode = 1;
					if (!en[i]->isCount) { en[i]->isCount = true; attackersCount++; }
					break;
				}
			}
		}
	}
}

void Player::getDamage(int dmg){
	health -= dmg;
}

void Player::draw(RenderWindow& w, bool debug) {
	w.draw(sprite);
	w.draw(maxHealthBar);
	w.draw(healthBar);

	if (debug) {
		w.draw(hitBox);
		w.draw(legHitBox);
	}
}

float Player::getplayercoordinateX() {	//этим методом будем забирать координату ’	
	return x;
}

float Player::getplayercoordinateY() {	//этим методом будем забирать координату ’	
	return y;
}

bool Player::isAlive(){
	return life;
}

FloatRect Player::getRect() {
	return FloatRect(x, y, w, h);
}