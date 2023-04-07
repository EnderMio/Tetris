#include<graphics.h>
#include"Tetris.h"

int main() {
    getRotateOffset();
    Graph* g = new Graph;
    auto flagNew{ true };
    auto flagMove{ 0 }, flagGame{ 0 };
    while (!flagGame) {
        g->check();
        if (kbhit())
            g->handleHit();
        if (flagNew) {
            flagNew = false;
            while (flagGame = g->addShape())
                if (flagGame == 1) break;
        }
        delay_fps(FPS);
        cleardevice();
        auto last = g->getLastShape();
        g->show(last);
        if (g->moveShape(last, 0, 1));
        else if (++flagMove >= 3) {
            flagMove = 0;
            flagNew = true;
            g->toOccupy(last);
        }
    }
    delete g;
    return 0;
}