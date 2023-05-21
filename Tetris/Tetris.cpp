#include"Tetris.h"

int offset[4][SHAPENUM][8] = { {
    { 1, 1, 1, -1, -1, -1, -1, 1 },//O
    { 0, 6, 0, 4, 0, 2, 0, 0 },//I
    { 0, -2, 0, 0, 2, 0, 4, 0 },//J
    { 0, 2, 0, 0, -2, 0, 0, -2 },//T
    { -2, 0, 0, 0, 0, -2, 2, -2 },//Z
    { 0, -2, 0, 0, -2, 0, -2, 2 },//S
    { 0, -2, 0, 0, -2, 0, -4, 0 }//L
} };

int Shape::numTotal = 0;

Shape::Shape(int x, int y, int color, int shape, int r) : x{ x }, y{ y }, color{ color }, shape{ shape }, rotate{ r } {
    id = numTotal++;
    shapeList.insert(make_pair(id, this));
    isSolid = false;
    speed = 1;
}

Shape::~Shape() {
    shapeList.erase(id);
    xyprintf(WIDTH * LENGTH / 2, 0, "Num: %d", shapeList.size());
}

void getRotateOffset() {
    Matrix m(0, -1, 1, 0), t(1, 0, 0, 1);
    for (auto j{ 1 }; j < 4; j++) {
        t = t * m;
        for (auto i{ 0 }; i < SHAPENUM; i++)
            for (auto k{ 0 }; k < 4; k++) {
                offset[j][i][k * 2] = t.getValue(0, 0) * offset[0][i][k * 2] + t.getValue(0, 1) * offset[0][i][k * 2 + 1];
                offset[j][i][k * 2 + 1] = t.getValue(1, 0) * offset[0][i][k * 2] + t.getValue(1, 1) * offset[0][i][k * 2 + 1];
            }
    }
}
void Shape::move(int x, int y) {
    this->x += x * 2;
    this->y += y * 2;
}

Shape& Shape::operator++(int dummy) {
    this->move(1, 0);
    return *this;
}
Shape& Shape::operator--(int dummy) {
    this->move(-1, 0);
    return *this;
}

vector<int> Shape::getNowPoints(int v) {
    vector<int> nowPoints;
    for (auto i{ 0 }; i < 4 * 2; i++)
        nowPoints.push_back(offset[(rotate + v) % 4][shape][i] + (i % 2 ? y : x));
    return nowPoints;
}

Graph::Graph() : score{ 0 }, gen{ mt19937(rd()) }, dis{ uniform_int_distribution<>(0, SHAPENUM - 1) }, disRotate{ uniform_int_distribution<>(0, 3) }, lastShape{ nullptr }, flagNew{ true }, flagMove{ 0 }, flagGame{ 0 } {
    initgraph(WIDTH * LENGTH, HEIGHT * LENGTH);
    setbkcolor(WHITE);
    for (auto i{ 0 }; i < WIDTH; i++)
        for (auto j{ 0 }; j < HEIGHT; j++)
            occupyState[i][j].first = false;
}
Graph::~Graph() {
    delete lastShape;
    getch();
    closegraph();
}
void Graph::loadSettings() {
    fs::path difficultyFilePath = "difficulty.txt";
    fs::path optionFilePath = "option.txt";
    if (!fs::exists(difficultyFilePath)) {
        std::ofstream difficultyFile(difficultyFilePath);
        difficultyFile << 1 << endl << 1;
        difficultyFile.close();
    }
    if (!fs::exists(optionFilePath)) {
        std::ofstream optionFile(optionFilePath);
        optionFile << 0 << ' ' << 1 << ' ' << 1 << endl;
        optionFile << 1 << ' ' << 1 << ' ' << 1 << endl;
        optionFile << 2 << ' ' << 1 << ' ' << 1 << endl;
        optionFile << 3 << ' ' << 1 << ' ' << 1 << endl;
        optionFile << 4 << ' ' << 1 << ' ' << 1 << endl;
        optionFile << 5 << ' ' << 1 << ' ' << 1 << endl;
        optionFile << 6 << ' ' << 1 << ' ' << 1 << endl;
        optionFile.close();
    }
    std::ifstream difficultyFile(difficultyFilePath);
    difficultyFile >> difficulty >> speedBase;
    difficultyFile.close();
    std::ifstream optionFile(optionFilePath);
    int id, isSolid, speed;
    while (optionFile >> id >> isSolid >> speed) {
        shapeList[id]->isSolid = isSolid;
        shapeList[id]->speed = speed;
    }
}
void Graph::toOccupy(Shape* s) {
    auto points = s->getNowPoints();
    for (auto i{ 0 }; i < 4 * 2; i += 2)
        if (!isOccupy(points[i], points[i + 1]))
            occupyState[points[i]][points[i + 1]] = make_pair(true, s->getColor());
}
bool Graph::moveShape(Shape* s, int x, int y) {
    auto points = s->getNowPoints();
    for (auto i{ 0 }; i < 4 * 2; i += 2)
        if (isOccupy(points[i] + x * 2, points[i + 1] + y * 2))
            return false;
    s->move(x, y);
    return true;
}
int Graph::addShape() {
    int res{ 0 };
    auto rotate{ disRotate(gen) };
    Shape* s;
    switch (dis(gen)) {
        case 0: s = new Square(WIDTH / 4 * 2, 0, rotate); break;
        case 1: s = new Straight(WIDTH / 4 * 2, 1, rotate); break;
        case 2: s = new JShape(WIDTH / 4 * 2, 3, rotate); break;
        case 3: s = new TShape(WIDTH / 4 * 2, 3, rotate); break;
        case 4: s = new ZShape(WIDTH / 4 * 2, 3, rotate); break;
        case 5: s = new SShape(WIDTH / 4 * 2, 3, rotate); break;
        case 6: s = new LShape(WIDTH / 4 * 2, 3, rotate); break;
        default: s = nullptr;
    }
    auto points = s->getNowPoints();
    for (auto i{ 0 }; i < 4 * 2; i += 2) {
        if (isOccupy(points[i], points[i + 1]))
            res = 1 + (points[i + 1] < 0);
        break;
    }
    if (!res) {
        delete lastShape;
        lastShape = s;
    }
    return res;
}
void Graph::show(Shape* s) {
    vector<int> points(10);
    for (auto i{ 0 }; i < WIDTH; i++)
        for (auto j{ 0 }; j < HEIGHT; j++)
            if (isOccupy(i, j)) {
                setfillcolor(occupyState[i][j].second);
                for (auto k{ 0 }; k < 10; k++)
                    points[k] = (normalOffset[k] + (k % 2 ? j : i)) * LENGTH;
                fillpoly(5, points.data());
            }
    if (s == nullptr) return;
    setfillcolor(s->getColor());
    auto p = s->getNowPoints();
    for (auto i{ 0 }; i < 4 * 2; i += 2) {
        for (auto j{ 0 }; j < 10; j++)
            points[j] = (normalOffset[j] + p[i + j % 2]) * LENGTH;
        fillpoly(5, points.data());
    }
}
void Graph::check() {
    auto count{ 0 };
    for (auto i{ 1 }; i < HEIGHT; i += 2) {
        for (auto j{ 1 }; j < WIDTH; j += 2)
            if (!occupyState[j][i].first) break;
            else if (j == WIDTH - 1) {
                count++;
                for (auto k{ i }; k > 1; k -= 2)
                    for (auto l{ 1 }; l < WIDTH; l += 2)
                        occupyState[l][k] = occupyState[l][k - 2];
                for (auto l{ 1 }; l < WIDTH; l += 2)
                    occupyState[l][1].first = false;
            }
    }
    score += count * count;
    xyprintf(0, 0, "Score: %d", score);
}
void Graph::handleHit() {
    char c = getch();
    switch (c) {
        case 'a': moveShape(lastShape, -1, 0); break;
        case 'd': moveShape(lastShape, 1, 0); break;
        case 's': while (moveShape(lastShape, 0, 1)); break;
        case 'w': setRotate(this, lastShape); break;
        case ' ': {
            while (true) {
                if (!kbhit())
                    break;
                getch();
            }
            getch();
            break;
        }
    }
}
void Graph::run() {
    while (!flagGame) {
        check();
        if (kbhit())
            handleHit();
        if (flagNew) {
            flagNew = false;
            while (flagGame = addShape())
                if (flagGame == 1) break;
        }
        delay_fps(FPS);
        cleardevice();
        auto last = getLastShape();
        // show(last);
        (*this) << last;
        if (moveShape(last, 0, 1));
        else if (++flagMove >= 3) {
            flagMove = 0;
            flagNew = true;
            toOccupy(last);
        }
    }
}

void setRotate(Graph* g, Shape* s) {
    auto p = s->getNowPoints(1);
    for (auto i{ 0 }; i < 4 * 2; i += 2)
        if (g->isOccupy(p[i], p[i + 1]))
            return;
    s->rotate = (s->rotate + 1) % 4;
}
Graph& operator<<(Graph& g, const Shape& s) {
    g.show(s);
    return g;
}