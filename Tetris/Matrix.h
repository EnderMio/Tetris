#pragma once

class Matrix {
    int a[4];
public:
    Matrix() = delete;
    Matrix(int _a, int _b, int _c, int _d) : a{ _a, _b, _c, _d } { }
    Matrix& operator=(const Matrix& m);
    Matrix operator*(const Matrix& m);
    int getValue(int x, int y);
};