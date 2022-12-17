#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"
#include "enemy.h"
#include "menu.h"
#include <vector>
#include <sstream>
using namespace sf;

class Game {
private:
    RenderWindow window;
    View view;
    Image map_image;
    Texture map;
    Sprite s_map;
    Clock clock;
    Font font;
    bool debugMode;
    int debugButtonDelay;
    std::vector<Enemy*> enemies;
    const int MAX_LEAF_SIZE = 300;
public:
    Game();
    ~Game();
    int run();
};

#endif