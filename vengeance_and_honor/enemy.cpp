#include "enemy.h"

Enemy::Enemy(String F, int id_en, float W, float H, float xlc, float ylc, float xrc, float yrc, Font f) {
	dx = 0; dy = 0; speed = 0.05;
	dir = 0;
	moveTimer = 0;
	attackTimer = 0;
	respawnTimer = 0;
	mode = 0;
	id = id_en;
	if (id == 0) {
		maxHealth = 50;
		damage = 5;
		name = "Skeleton";
		level = 1;
		expReward = 40;
		ANIM_WALKING_MAX_FRAMES = 6;
		ANIM_ATTACK_MAX_FRAMES = 8;
		ANIM_DEATH_MAX_FRAMES = 7;
	}else if (id == 1) {
		maxHealth = 500;
		damage = 25;
		name = "Skeleton king";
		level = 10;
		expReward = 500;
		ANIM_WALKING_MAX_FRAMES = 10;
		ANIM_ATTACK_MAX_FRAMES = 10;
		ANIM_DEATH_MAX_FRAMES = 13;
	}
	health = maxHealth;
	animDir = 0;
	animType = 0;
	CurrentFrame = 0;
	isCount = false;
	life = true;
	
	File = F;
	w = W; h = H;
	// Texture load
	image.loadFromFile("images/" + File);
	image.createMaskFromColor(Color(41, 33, 59));
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	// Left corners(LC) and right corners(RC) for wandering
	xLC = xlc;
	yLC = ylc;
	xRC = xrc;
	yRC = yrc;
	x = rand() % (int)(xRC - xLC - 32 + 1) + (int)xLC;
	y = rand() % (int)(yRC - yLC - 32 * 2 + 1) + (int)yLC;
	// X and y destination coords
	xD = x;
	yD = y;

	
	// Sptite/hitboxes/healthbar set
	sprite.setTextureRect(IntRect(0, 0, w, h));

	hitBox.setSize(sf::Vector2f(w, h));
	hitBox.setFillColor(sf::Color(255, 0, 0, 0));
	hitBox.setOutlineColor(sf::Color(0, 0, 255));
	hitBox.setPosition(x, y);
	hitBox.setOutlineThickness(1);

	wanderingBox.setSize(sf::Vector2f(xRC - xLC, yRC - yLC));
	wanderingBox.setFillColor(sf::Color(255, 0, 0, 0));
	wanderingBox.setOutlineColor(sf::Color(0, 255, 0));
	wanderingBox.setPosition(xLC, yLC);
	wanderingBox.setOutlineThickness(1);

	healthBar.setSize(sf::Vector2f(w * health / maxHealth, 3));
	healthBar.setFillColor(sf::Color(255, 0, 0));
	healthBar.setPosition(x, y - 5);

	legHitBox.setRadius(3);
	legHitBox.setPosition(x + w / 2, y + h);
	legHitBox.setFillColor(sf::Color::Red);
}

void Enemy::update(float time, float playerX, float playerY, float playerW, float playerH, int& pl_hp, int& pl_att_c) {
	// Timers decreasing
	if (moveTimer > 0) { moveTimer--; }
	if (attackTimer > 0) { attackTimer--; }
	if (respawnTimer > 0) { respawnTimer--; }

	if (life) {
		if (mode == 0) { // Wandering in random point of room
			if ((int)x / 32 == (int)xD / 32 && (int)y / 32 == (int)yD / 32 && moveTimer == 0) { // If we reach destination point
				moveTimer = 500;
				xD = rand() % (int)(xRC - xLC - 32 + 1) + (int)xLC;
				yD = rand() % (int)(yRC - yLC - 32 * 2 + 1) + (int)yLC;
			}
			else {
				if (moveTimer == 0) { // Go to destination point
					if ((int)x / 32 < (int)xD / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 0; animType = 0;
						x += speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
					else if ((int)x / 32 > (int)xD / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 1; animType = 0;
						x -= speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
					else if ((int)y / 32 < (int)yD / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 3; animType = 0;
						y += speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
					else if ((int)y / 32 > (int)yD / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 2; animType = 0;
						y -= speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
				}
			}
		}else{
			if ((playerX + playerW / 2 + 3) < xLC || (playerX + playerW / 2 + 3) > xRC || (playerY + playerH - 15) < yLC || (playerY + playerH - 15) > yRC) { // Player went out of enemy's room, so start wandering
				mode = 0;
				health = maxHealth;
				if (isCount) {
					isCount = false;
					pl_att_c--;
				}
			}else{ // We are near player, so attack him
				if ((int)x / 32 == (int)playerX / 32 && (int)y / 32 == (int)playerY / 32) {
					if (attackTimer == 0) {
						animType = 1;
						CurrentFrame = 0;
						pl_hp -= damage;
						attackTimer = 300;
						if (pl_hp <= 0) {
							mode = 0;
							health = maxHealth;
							if (isCount) {
								isCount = false;
								pl_att_c--;
							}
						}
					}
				}else{ // Chasing player
					if ((int)x / 32 < (int)playerX / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 0; animType = 0;
						x += speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
					else if ((int)x / 32 > (int)playerX / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 1; animType = 0;
						x -= speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
					else if ((int)y / 32 < (int)playerY / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 3; animType = 0;
						y += speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
					else if ((int)y / 32 > (int)playerY / 32) { 
						if (animType != 0) { CurrentFrame = 0; }
						animDir = 2; animType = 0;
						y -= speed * time;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > ANIM_WALKING_MAX_FRAMES) CurrentFrame -= ANIM_WALKING_MAX_FRAMES;
						sprite.setTextureRect(IntRect(int(w) * int(CurrentFrame), int(h) * animDir, int(w), int(h)));
					}
				}
			}
		}
	}

	// Death state and animation
	if (health <= 0) {
		life = false;
		if (isCount) {
			isCount = false;
			pl_att_c--;
		}
		if (animType != 2) {
			animType = 2;
			CurrentFrame = 0;
			respawnTimer = 2000;
			mode = 0;
		}
		if (CurrentFrame <= ANIM_DEATH_MAX_FRAMES) {
			CurrentFrame += 0.005 * time;
			sprite.setTextureRect(IntRect(int(w) * (int(CurrentFrame) + 14), int(h) * animDir, int(w), int(h)));
		}
	}
	// Attack animation
	if (animType == 1) {
		CurrentFrame += 0.005 * time;
		sprite.setTextureRect(IntRect(int(w)* (int(CurrentFrame) + 6), int(h)* animDir, int(w), int(h)));
		if (CurrentFrame > ANIM_ATTACK_MAX_FRAMES) {
			animType = 0;
			CurrentFrame = 0;
		}
	}
	// Respawn
	if (respawnTimer == 1) {
		life = true;
		health = maxHealth;
		moveTimer = 500;
		xD = rand() % (int)(xRC - xLC - 32 + 1) + (int)xLC;
		yD = rand() % (int)(yRC - yLC - 32 * 2 + 1) + (int)yLC;
		CurrentFrame = 0;
		animType = 0;
		animDir = 0;
		sprite.setTextureRect(IntRect(0, 0, int(w), int(h)));
		
	}
	// Sprites sets
	healthBar.setSize(sf::Vector2f(w * health / maxHealth, 3));
	healthBar.setPosition(x, y - 5);
	sprite.setPosition(x, y);
	hitBox.setPosition(x, y);
	legHitBox.setPosition(x + w / 2, y + h);
}

float Enemy::getenemycoordinateX() {
	return x;
}

float Enemy::getenemycoordinateY() {	
	return y;
}

void Enemy::getDamage(int dmg, bool wellRiddle, bool tableRiddle){
	if(id == 0 || (id == 1 && wellRiddle && tableRiddle))
	health -= dmg;
}

void Enemy::draw(RenderWindow& w, bool debug){
	w.draw(sprite);

	if (life) {
		w.draw(healthBar);
	}

	if (debug) {
		w.draw(hitBox);
		w.draw(legHitBox);
		w.draw(wanderingBox);
	}
}

FloatRect Enemy::getRect() {
	return FloatRect(x, y, w, h);
}