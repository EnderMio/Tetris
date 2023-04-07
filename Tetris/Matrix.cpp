#include"Matrix.h"

Matrix& Matrix::operator=(const Matrix& m) {
    for (auto i{ 0 }; i < 4; i++)
        a[i] = m.a[i];
    return *this;
}
Matrix Matrix::operator*(const Matrix& m) {
    return Matrix(a[0] * m.a[0] + a[1] * m.a[2], a[0] * m.a[1] + a[1] * m.a[3], a[2] * m.a[0] + a[3] * m.a[2], a[2] * m.a[1] + a[3] * m.a[3]);
}
int Matrix::getValue(int x, int y) {
    return a[x * 2 + y];
}