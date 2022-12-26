#ifndef ENEMY_H
#define ENEMY_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace sf;

class Enemy {
private: float x, y;
public:
	float w, h, dx, dy, speed;
	float xLC, yLC, xRC, yRC; //Corners of area, where enemy walking(LC - Left Corner, RC - Right Corner)
	float xD, yD; //Destination x, y
	int dir;
	int moveTimer;
	int attackTimer;
	int respawnTimer;
	int mode; // 0 - walking, 1 - chasing player
	int maxHealth;
	int health;
	int damage;
	String name;
	int level;
	bool life;
	bool isCount; // Is enemy cointed for player's attackers count
	// Animation vars
	float CurrentFrame;
	int animDir; // Walking direction
	int animType; // 0 - walking 1 - attack 2 - death
	int ANIM_WALKING_MAX_FRAMES;
	int ANIM_ATTACK_MAX_FRAMES = 8;
	int ANIM_DEATH_MAX_FRAMES = 7;
	// Textures
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	RectangleShape hitBox;
	RectangleShape wanderingBox;
	RectangleShape healthBar;
	CircleShape legHitBox;
	// Functions
	Enemy(String F, int id, float W, float H, float xlc, float ylc, float xrc, float yrc, Font f);
	void update(float time, float playerX, float playerY, float playerW, float playerH, int& pl_hp, int& pl_att_c);
	float getenemycoordinateX();
	float getenemycoordinateY();
	void getDamage(int dmg);
	void draw(RenderWindow& w, bool debug);
	FloatRect getRect();
};

#endif