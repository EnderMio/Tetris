#pragma once
#include<graphics.h>
#include<conio.h>
#include<vector>
#include<array>
#include<random>
#include"Matrix.h"
using std::vector, std::array, std::random_device, std::mt19937, std::uniform_int_distribution, std::pair, std::make_pair;

static const auto WIDTH{ 30 }, HEIGHT{ 40 }, FPS{ 5 }, LENGTH{ 10 }, SHAPENUM{ 7 };
static const int normalOffset[10] = { -1, -1, -1, 1, 1, 1, 1, -1, -1, -1 };
extern int offset[4][SHAPENUM][8];

void getRotateOffset();

class Graph;
class Shape {
public:
    int getColor() { return color; }
    vector<int> getNowPoints(int v = 0);
    friend void setRotate(Graph*, Shape*);
    void move(int x, int y);
    void setHeight(int y) { this->y = y; }
protected:
    int x, y, color, shape, rotate;
    Shape(int x, int y, int color, int shape, int r) : x{ x }, y{ y }, color{ color }, shape{ shape }, rotate{ r } { }
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
    int score;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> dis;
    uniform_int_distribution<> disRotate;
    Shape* lastShape;
    array<array<pair<bool, int>, HEIGHT>, WIDTH> occupyState;
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
};

void setRotate(Graph* g, Shape* s);