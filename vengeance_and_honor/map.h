#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;

const int HEIGHT_MAP = 90;
const int WIDTH_MAP = 140;

class Leaf { // Class for creating BSP-structure of map
public:
    int x, y, width, height;
    Leaf* rightChild = nullptr;
    bool isRightChildNull = true;
    Leaf* leftChild = nullptr;
    bool isLeftChildNull = true;

    vector<vector<int>> halls;
    vector<int> room = { -1, -1, -1, -1 };  // x, y, width, height
    bool isRoomCreate = false;

    Leaf(int X, int Y, int W, int H);
    bool split();
    void createRoom();
    vector<int> getRoom();
    void createHall(vector<int> l, vector<int> r);
private:
    const int MIN_LEAF_SIZE = 200;
};

#endif