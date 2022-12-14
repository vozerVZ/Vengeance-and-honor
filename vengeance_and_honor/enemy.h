#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include "player.h"
using namespace sf;

class Enemy {
private: float x, y;
public:
	float w, h, dx, dy, speed;
	float xLC, yLC, xRC, yRC; //Corners of area, where enemy walking(LC - Left Corner, RC - Right Corner)
	float xD, yD; //Destination x, y
	int dir = 0;
	int moveTimer = 0;
	int attackTimer = 0;
	int respawnTimer = 0;
	int mode = 0; // 0 - walking, 1 - chasing player
	int maxHealth = 50;
	int health = maxHealth;
	int damage = 5;
	String name = "Skeleton";
	int level = 1;
	bool life = true;
	bool isCount = false; // Is enemy cointed for player's attackers count
	//Animation
	float CurrentFrame;
	int animDir = 0; // Walking direction
	int animType = 0; // 0 - walking 1 - attack 2 - death
	const int ANIM_WALKING_MAX_FRAMES = 6;
	const int ANIM_ATTACK_MAX_FRAMES = 8;
	const int ANIM_DEATH_MAX_FRAMES = 7;
	//Textures
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	RectangleShape hitBox;
	RectangleShape wanderingBox;
	RectangleShape healthBar;
	CircleShape legHitBox;
	//Methods
	Enemy(String F, float W, float H, float xlc, float ylc, float xrc, float yrc, Font f);
	void update(float time, float playerX, float playerY, Player& pl);
	float getenemycoordinateX();
	float getenemycoordinateY();
	void getDamage(int dmg);
	void draw(RenderWindow& w, bool debug);
	FloatRect getRect();
};

#endif