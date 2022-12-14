#ifndef PLAYER_H
#define PLAYER_H

#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;

class Player {
private: float x, y;
public:
	float w, h, dx, dy, speed;
	int dir = 0;
	int maxHealth = 100;
	int health = maxHealth;
	int damage = 20;
	bool life = true;
	int attackersCount = 0;
	//Timers
	int attackTimer = 0;
	int respawnTimer = 0;
	int idleTimer = 0;
	int healTimer = 0;
	//Animation
	float CurrentFrame;
	int animDir = 0; // Walking direction
	int animType = 0; // 0 - walking 1 - idle 2 - attack 3 - death
	const int ANIM_WALKING_MAX_FRAMES = 8;
	const int ANIM_IDLE_MAX_FRAMES = 5;
	const int ANIM_ATTACK_MAX_FRAMES = 6;
	const int ANIM_DEATH_MAX_FRAMES = 4;
	//Sprites
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	RectangleShape hitBox;
	CircleShape legHitBox;
	RectangleShape maxHealthBar;
	RectangleShape healthBar;
	//Methods
	Player(String F, float X, float Y, float W, float H);
	void update(float time);
	void move(const int tileMap[], size_t m, size_t n, float t);
	void getDamage(int dmg);
	void draw(RenderWindow& w, bool debug);
	float getplayercoordinateX();
	float getplayercoordinateY();
	bool isAlive();
	FloatRect getRect();
};

#endif