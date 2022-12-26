#include "map.h"

int tileMap[90][140];
vector<Leaf*> leafs;
bool isWellsCreate = false;

int xWell = -1;
int yWell = -1;

Leaf::Leaf(int X, int Y, int W, int H) {
    x = X;
    y = Y;
    width = W;
    height = H;
}

bool Leaf::split() { // Randomly split leaf into two smaller
    if (!isRightChildNull || !isLeftChildNull) {
        return false;
    }

    bool splitMode = (rand() % 100) > 50;

    if (width > height && width / height >= 1.25) {
        splitMode = false;
    }
    else if (height > width && height / width >= 1.25) {
        splitMode = true;
    }

    int max = (splitMode ? height : width) - MIN_LEAF_SIZE;

    if (max < MIN_LEAF_SIZE) {
        return false;
    }

    if (max == MIN_LEAF_SIZE) {
        max++;
    }

    int cutLine = (rand() % (max - MIN_LEAF_SIZE)) + MIN_LEAF_SIZE;

    if (splitMode) {
        leftChild = new Leaf(x, y, width, cutLine);
        rightChild = new Leaf(x, y + cutLine, width, height - cutLine);
    }
    else {
        leftChild = new Leaf(x, y, cutLine, height);
        rightChild = new Leaf(x + cutLine, y, width - cutLine, height);
    }
    isRightChildNull = false;
    isLeftChildNull = false;

    return true;
}

void Leaf::createRoom() { 
    // If fe dont have children(=>smallest leaf), create room inside
    // Else recursively call this function on children
    /*if (isLeftChildNull && isRightChildNull) {
        int xRand = 10 + rand() % (width / 5);
        int yRand = 10 + rand() % (height / 5);
        room[0] = x + xRand;
        room[1] = y + yRand;
        room[2] = (width / 2) + rand() % (width - xRand - (width / 2) - 10);
        room[3] = (height / 2) + rand() % (height - yRand - (height / 2) - 10);
        isRoomCreate = true;
    }*/
    if (!isLeftChildNull || !isRightChildNull) {
        if (!isLeftChildNull) { leftChild->createRoom(); }
        if (!isRightChildNull) { rightChild->createRoom(); }

        if (!isRightChildNull && !isLeftChildNull) {
            createHall(leftChild->getRoom(), rightChild->getRoom());
        }

    }
    else {
        int xRand = 10 + rand() % (width / 5);
        int yRand = 10 + rand() % (height / 5);
        room[0] = x + xRand;
        room[1] = y + yRand;
        room[2] = (width / 2) + rand() % (width - xRand - (width / 2) - 10);
        room[3] = (height / 2) + rand() % (height - yRand - (height / 2) - 10);
        isRoomCreate = true;
    }

}

vector<int> Leaf::getRoom() { // Get room from deepest parts
    if (isRoomCreate) {
        return room;
    }
    else {
        vector<int> lRoom = { -1, -1, -1, -1 };
        vector<int> rRoom = { -1, -1, -1, -1 };

        if (!isLeftChildNull) { lRoom = leftChild->getRoom(); }
        if (!isRightChildNull) { rRoom = rightChild->getRoom(); }

        if (lRoom[0] == -1 and rRoom[0] == -1) {
            vector<int> returnOne = { -1, -1, -1, -1 };
            return returnOne;
        }
        else if (lRoom[0] == -1) {
            return rRoom;
        }
        else if (rRoom[0] == -1) {
            return lRoom;
        }
        else {
            return lRoom;
        }
    }
}

void Leaf::createHall(vector<int> l, vector<int> r) { // Create hall between two rooms
    vector<int> point1 = { (l[0] + 10) + rand() % (l[2] - 10), (l[1] + 10) + rand() % (l[3] - 10) };
    vector<int> point2 = { (r[0] + 10) + rand() % (r[2] - 10), (r[1] + 10) + rand() % (r[3] - 10) };

    int w = point2[0] - point1[0];
    int h = point2[1] - point1[1];

    int hallThick = 8;

    if (w < 0) {
        if (h < 0) {
            if ((rand() % 100) < 50) {
                halls.push_back({ point2[0], point1[1], abs(w), hallThick });
                halls.push_back({ point2[0], point2[1], hallThick, abs(h) });
            }
            else {
                halls.push_back({ point2[0], point2[1], abs(w), hallThick });
                halls.push_back({ point1[0], point2[1], hallThick, abs(h) });
            }
        }
        else if (h > 0) {
            if ((rand() % 100) < 50) {
                halls.push_back({ point2[0], point1[1], abs(w), hallThick });
                halls.push_back({ point2[0], point1[1], hallThick, abs(h) });
            }
            else {
                halls.push_back({ point2[0], point2[1], abs(w), hallThick });
                halls.push_back({ point1[0], point1[1], hallThick, abs(h) });
            }
        }
        else {
            halls.push_back({ point2[0], point2[1], abs(w), hallThick });
        }
    }
    else if (w > 0) {
        if (h < 0) {
            if ((rand() % 100) < 50) {
                halls.push_back({ point1[0], point2[1], abs(w), hallThick });
                halls.push_back({ point1[0], point2[1], hallThick, abs(h) });
            }
            else {
                halls.push_back({ point1[0], point1[1], abs(w), hallThick });
                halls.push_back({ point2[0], point2[1], hallThick, abs(h) });
            }
        }
        else if (h > 0) {
            if ((rand() % 100) < 50) {
                halls.push_back({ point1[0], point1[1], abs(w), hallThick });
                halls.push_back({ point2[0], point1[1], hallThick, abs(h) });
            }
            else {
                halls.push_back({ point1[0], point2[1], abs(w), hallThick });
                halls.push_back({ point1[0], point1[1], hallThick, abs(h) });
            }
        }
        else {
            halls.push_back({ point1[0], point1[1], abs(w), hallThick });
        }
    }
    else {
        if (h < 0) {
            halls.push_back({ point2[0], point2[1], hallThick, abs(h) });
        }
        else if (h > 0) {
            halls.push_back({ point1[0], point1[1], hallThick, abs(h) });
        }
    }

}

void createMap(vector<Enemy*>& en, Font f){
    Leaf root(0, 0, 1400, 900);
    leafs.push_back(&root);

    bool splitCycle = true;

    while (splitCycle) {
        splitCycle = false;

        for (int i = 0; i < leafs.size(); i++) {
            if (leafs[i]->isRightChildNull && leafs[i]->isLeftChildNull) {
                if (leafs[i]->width > root.MAX_LEAF_SIZE || leafs[i]->height > root.MAX_LEAF_SIZE || (rand() % 100) > 25) {
                    if (leafs[i]->split()) {
                        leafs.push_back(leafs[i]->leftChild);
                        leafs.push_back(leafs[i]->rightChild);
                        splitCycle = true;
                    }
                }
            }
        }
    }

    root.createRoom();

    for (auto l : leafs) { // Transfer from classes to map
        if (6 < l->width < 20 && 6 < l->height < 20) {
            if (l->isRoomCreate) {
                for (int i = l->room[0] / 10; i < (l->room[0] + l->room[2]) / 10 + 1; i++) {
                    for (int j = l->room[1] / 10; j < (l->room[1] + l->room[3]) / 10 + 1; j++) {
                        tileMap[j][i] = 1;
                    }
                }
                
                if (!isWellsCreate) { // Crete wells in this room
                    tileMap[l->room[1] / 10 + 1][l->room[0] / 10 + 1] = 3;
                    tileMap[l->room[1] / 10 + 1][l->room[0] / 10 + 3] = 3;
                    tileMap[l->room[1] / 10 + 1][l->room[0] / 10 + 5] = 3;
                    tileMap[l->room[1] / 10 + 1][l->room[0] / 10 + 7] = 3;
                    tileMap[l->room[1] / 10 + 1][l->room[0] / 10 + 9] = 3;
                    
                    xWell = l->room[0] / 10 + 1;
                    yWell = l->room[1] / 10 + 1;

                    isWellsCreate = true;
                }

                // Create enemies in this room
                en.push_back(new Enemy("enemy.png", 96.0, 96.0, l->room[0] / 10 * 32, l->room[1] / 10 * 32, ((l->room[0] + l->room[2]) / 10 + 1) * 32, ((l->room[1] + l->room[3]) / 10 + 1) * 32, f));
                en.push_back(new Enemy("enemy.png", 96.0, 96.0, l->room[0] / 10 * 32, l->room[1] / 10 * 32, ((l->room[0] + l->room[2]) / 10 + 1) * 32, ((l->room[1] + l->room[3]) / 10 + 1) * 32, f));
                en.push_back(new Enemy("enemy.png", 96.0, 96.0, l->room[0] / 10 * 32, l->room[1] / 10 * 32, ((l->room[0] + l->room[2]) / 10 + 1) * 32, ((l->room[1] + l->room[3]) / 10 + 1) * 32, f));
            }
            if (l->halls.size() > 0) {
                for (auto k : l->halls) {
                    for (int i = k[0] / 10; i < (k[0] + k[2]) / 10 + 1; i++) {
                        for (int j = k[1] / 10; j < (k[1] + k[3]) / 10 + 1; j++) {
                            tileMap[j][i] = 1;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < 90; i++) { // creating walls
        for (int j = 0; j < 140; j++) {
            if (tileMap[i][j] == 0) {
                if (i > 0 && tileMap[i - 1][j] == 1) { tileMap[i][j] = 2; }
                else if (i < 89 && tileMap[i + 1][j] == 1) { tileMap[i][j] = 2; }
                else if (j > 0 && tileMap[i][j - 1] == 1) { tileMap[i][j] = 2; }
                else if (j < 139 && tileMap[i][j + 1] == 1) { tileMap[i][j] = 2; }
                else if (i < 89 && j < 139 && tileMap[i + 1][j + 1] == 1) { tileMap[i][j] = 2; }
                else if (i > 0 && j < 139 && tileMap[i - 1][j + 1] == 1) { tileMap[i][j] = 2; }
                else if (i < 89 && j > 0 && tileMap[i + 1][j - 1] == 1) { tileMap[i][j] = 2; }
                else if (i > 0 && j > 0 && tileMap[i - 1][j - 1] == 1) { tileMap[i][j] = 2; }
            }
        }
    }

    /*for (int i = 0; i < 90; i++) {
        for (int j = 0; j < 140; j++) {
            cout << tileMap[i][j];
        }
        cout << endl;
    }
    */
}