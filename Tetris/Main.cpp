#include<graphics.h>
#include"Tetris.h"

int main() {
    getRotateOffset();
    Graph* g = new Graph;
    g->run();
    delete g;
    return 0;
}