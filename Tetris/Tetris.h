#pragma once
#include<graphics.h>
#include<conio.h>
#include<vector>
#include<array>
#include<random>
#include<map>
#include<fstream>
#include<filesystem>
#include"Matrix.h"
using std::vector, std::array, std::random_device, std::mt19937, std::uniform_int_distribution, std::pair, std::make_pair, std::map, std::fstream, std::endl;

namespace fs = std::filesystem;

static const auto WIDTH{ 30 }, HEIGHT{ 40 }, FPS{ 2 }, LENGTH{ 10 }, SHAPENUM{ 7 };
static const int normalOffset[10] = { -1, -1, -1, 1, 1, 1, 1, -1, -1, -1 };
extern int offset[4][SHAPENUM][8];

void getRotateOffset();

class Graph;
class Shape {
public:
    int getColor() { return color; }
    vector<int> getNowPoints(int v = 0);
    map<int, Shape*> shapeList;
    friend void setRotate(Graph*, Shape*);
    void move(int x, int y);
    void setHeight(int y) { this->y = y; }
    ~Shape();
    Shape& operator++(int dummy = 0);
    Shape& operator--(int dummy = 0);
protected:
    int x, y, color, shape, rotate, id, speed;
    bool isSolid;
    static int numTotal;
    Shape(int x, int y, int color, int shape, int r);
};
class Square : public Shape {
public:
    Square(int x, int y, int r) : Shape(x, y, BLACK, 0, r) { }
};
class Straight : public Shape {
public:
    Straight(int x, int y, int r) : Shape(x - 1, y, RED, 1, r) { }
};
class JShape : public Shape {
public:
    JShape(int x, int y, int r) : Shape(x - 1, y, YELLOW, 2, r) { }
};
class TShape : public Shape {
public:
    TShape(int x, int y, int r) : Shape(x - 1, y, GREEN, 3, r) { }
};
class ZShape : public Shape {
public:
    ZShape(int x, int y, int r) : Shape(x - 1, y, BLUE, 4, r) { }
};
class SShape : public Shape {
public:
    SShape(int x, int y, int r) : Shape(x - 1, y, CYAN, 5, r) { }
};
class LShape : public Shape {
public:
    LShape(int x, int y, int r) : Shape(x - 1, y, MAGENTA, 6, r) { }
};

class Graph {
    int score, flagMove, flagGame, difficulty, speedBase;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> dis;
    uniform_int_distribution<> disRotate;
    Shape* lastShape;
    array<array<pair<bool, int>, HEIGHT>, WIDTH> occupyState;
    bool flagNew;
public:
    Graph();
    ~Graph();
    void toOccupy(Shape* s);
    bool moveShape(Shape* s, int x, int y);
    bool isOccupy(int x, int y) { return x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || occupyState[x][y].first; }
    Shape* getLastShape() { return lastShape; }
    int addShape();
    void show(Shape* s);
    void check();
    void handleHit();
    void run();
    void loadSettings();
    friend Graph& operator<<(Graph& g, const Shape& s);
};

void setRotate(Graph* g, Shape* s);
Graph& operator<<(Graph& g, const Shape& s);