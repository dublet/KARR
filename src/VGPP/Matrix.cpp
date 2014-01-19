#include "Matrix.h"

#include <cmath>
#include <utility>

#include <string.h>

using namespace VGPP;
using namespace std;

Matrix::Matrix() {
    makeIdentity();
}

Matrix::Matrix(const VGfloat *otherMatrix) {
    memcpy(&_m[0], otherMatrix, 9 * sizeof(VGfloat));
}

Matrix::~Matrix() {
}

void Matrix::makeIdentity() {
    sx() = sy() = w2() = 1;
    shx() = shy() = tx() = ty() = w0() = w1() = 0;
}

void Matrix::transpose() {
    /**
     * For matrix:
     * [ sx  shx tx
     *   shy sy  ty
     *   w0  w1  w2 ] 
     *  Change from:
     *   0   1    2   3    4   5   6   7   8
     * [ sx, shy, w0, shx, sy, w1, tx, ty, w2 ]
     * to:
     * [ sx, shx, tx, shy, sy, ty, w0, w1, w2 ]
     *   0   3    6   1    4   7   2   5   8
     */
    swap(_m[1], _m[3]);
    swap(_m[2], _m[6]);
    swap(_m[5], _m[7]);
}

void Matrix::rotate(VGfloat degrees) {
    sx() *= cosf(degrees);
    shx() *= -sinf(degrees);
    shy() *= sinf(degrees);
    sy() *= cosf(degrees);
}

void Matrix::translate(VGfloat newTx, VGfloat newTy) {
    Matrix m1;
    m1.tx() = newTx;
    m1.ty() = newTy;
    *this *= m1;
}

void Matrix::scale(VGfloat newSx, VGfloat newSy) {
    Matrix m1;
    m1.sx() = newSx;
    m1.sy() = newSy;
    *this *= m1;
}

Matrix &Matrix::operator+=(const VGfloat *rhs) {
    _m[0] += rhs[0]; _m[1] += rhs[1]; _m[2] += rhs[2];
    _m[3] += rhs[3]; _m[4] += rhs[4]; _m[5] += rhs[5];
    _m[6] += rhs[6]; _m[7] += rhs[7]; _m[8] += rhs[8];

    return *this;
}

Matrix &Matrix::operator*=(const VGfloat *rhs) {
    _m[0] *= rhs[0]; _m[1] *= rhs[1]; _m[2] *= rhs[2];
    _m[3] *= rhs[3]; _m[4] *= rhs[4]; _m[5] *= rhs[5];
    _m[6] *= rhs[6]; _m[7] *= rhs[7]; _m[8] *= rhs[8];

    return *this;
}

Matrix &Matrix::operator+=(const Matrix &rhs) {
    return *this += rhs._m;
}

Matrix &Matrix::operator*=(const Matrix &rhs) {
    return *this *= rhs._m;
}
