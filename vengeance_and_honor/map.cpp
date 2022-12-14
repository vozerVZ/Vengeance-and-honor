#include "map.h"

Leaf::Leaf(int X, int Y, int W, int H) {
    x = X;
    y = Y;
    width = W;
    height = H;
}

bool Leaf::split() {
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

vector<int> Leaf::getRoom() {
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

void Leaf::createHall(vector<int> l, vector<int> r) {
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