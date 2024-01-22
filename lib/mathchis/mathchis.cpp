#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <math.h>
#include "mathchis.h"
#include <cstdio>
#include <cstring>

using namespace std;

// --- MATRIX DEFINITIONS --- //

MATHCHIS::matrix::matrix(int n_d, int m_d) {
    if (n_d < 1 || m_d < 1)
        throw std::invalid_argument("Cannot create a matrix with size 0 or less");

    elems.reserve(n_d*m_d);
    n = n_d;
    m = m_d;
}

MATHCHIS::matrix::matrix(int n_d, int m_d, std::vector<float> o_elems) {
    if (o_elems.size() < n_d*m_d)
        throw std::invalid_argument("Supplied elements do not match matrix size");
    if (n_d < 1 || m_d < 1)
        throw std::invalid_argument("Cannot create a matrix with size 0 or less");

    elems = o_elems;
    n = n_d;
    m = m_d;
}

MATHCHIS::matrix MATHCHIS::matrix::operator+(const MATHCHIS::matrix &obj) const {
    if (n != obj.n || m != obj.m)
        throw std::invalid_argument("Cannot perform addition on matrices of different size");

    matrix n_matrix = matrix(n, m);
    for (int i = 0; i < elems.size(); i++) {
        n_matrix.elems.push_back(elems[i] + obj.elems[i]);
    }

    return n_matrix;
}

MATHCHIS::matrix &MATHCHIS::matrix::operator+=(const MATHCHIS::matrix &obj) {
    if (n != obj.n || m != obj.m)
        throw std::invalid_argument("Cannot perform addition on matrices of different size");

    for (int i = 0; i < elems.size(); i++) {
        elems[i] += obj.elems[i];
    }

    return *this;
}

MATHCHIS::matrix MATHCHIS::matrix::operator-(const MATHCHIS::matrix &obj) const {
    if (n != obj.n || m != obj.m)
        throw std::invalid_argument("Cannot perform subtraction on matrices of different size");

    matrix n_matrix = matrix(n, m);
    for (int i = 0; i < elems.size(); i++) {
        n_matrix.elems.push_back(elems[i] - obj.elems[i]);
    }

    return n_matrix;
}

MATHCHIS::matrix &MATHCHIS::matrix::operator-=(const MATHCHIS::matrix &obj) {
    if (n != obj.n || m != obj.m)
        throw std::invalid_argument("Cannot perform addition on matrices of different size");

    for (int i = 0; i < elems.size(); i++) {
        elems[i] -= obj.elems[i];
    }

    return *this;
}

MATHCHIS::matrix MATHCHIS::matrix::operator*(const MATHCHIS::matrix &obj) const {
    if (m != obj.n)
        throw std::invalid_argument("Cannot multiply matrices with incongruent size");

    matrix n_matrix = matrix(n, obj.m);

    for (int m2_i = 0; m2_i < obj.m; m2_i++) {
        for (int n_i = 0; n_i < n; n_i++) {
            float net = 0;
            for (int i = 0; i < m; i++) {
                net += elems[n_i + i * n] * obj.elems[m2_i*obj.n + i];
            }
            n_matrix.elems.push_back(net);
        }
    }

    return n_matrix;
}

MATHCHIS::matrix &MATHCHIS::matrix::operator*=(const MATHCHIS::matrix &obj) {
    matrix n_matrix = *this*obj;
    elems = n_matrix.elems;
    n = n_matrix.n;
    m = n_matrix.m;

    return *this;
}

// --- VECTOR DEFINITIONS --- //

MATHCHIS::vector::vector(std::vector<float> o_elems) {
    elems = o_elems;
}

MATHCHIS::vector::vector(float x, float y, float z) {
    elems.reserve(3);
    elems.push_back(x);
    elems.push_back(y);
    elems.push_back(z);
}

MATHCHIS::vector::vector(float o) {
    elems.reserve(3);
    for (int i = 0; i < 3; i++)
        elems.push_back(o);
}

MATHCHIS::vector::vector() {
    elems.reserve(3);
    for (int i = 0; i < 3; i++)
        elems.push_back(0);
}


MATHCHIS::vector MATHCHIS::vector::operator+(const MATHCHIS::vector &obj) const {
    if (elems.size() != obj.elems.size())
        throw std::invalid_argument("Cannot perform addition on vectors of different dimensionality");

    std::vector<float> tElems;
    tElems.reserve(elems.size());

    for (int i = 0; i < elems.size(); i++)
        tElems.push_back(elems[i] + obj.elems[i]);

    return vector(tElems);
}

MATHCHIS::vector MATHCHIS::vector::operator-(const MATHCHIS::vector &obj) const {
    if (elems.size() != obj.elems.size())
        throw std::invalid_argument("Cannot perform subtraction on vectors of different dimensionality");

    std::vector<float> tElems;
    tElems.reserve(elems.size());

    for (int i = 0; i < elems.size(); i++)
        tElems.push_back(elems[i] - obj.elems[i]);

    return vector(tElems);
}

MATHCHIS::vector MATHCHIS::vector::operator*(const float &obj) const {
    std::vector<float> tElems;
    tElems.reserve(elems.size());

    for (float elem : elems)
        tElems.push_back(elem * obj);

    return vector(tElems);
}

MATHCHIS::vector MATHCHIS::vector::operator/(const float &obj) const {
    std::vector<float> tElems;
    tElems.reserve(elems.size());

    for (float elem : elems)
        tElems.push_back(elem / obj);

    return vector(tElems);
}

// -------------------------------------------------------------------------------------------- //
// Assignment Operators
// -------------------------------------------------------------------------------------------- //

MATHCHIS::vector& MATHCHIS::vector::operator+=(const MATHCHIS::vector &obj) {
    if (elems.size() != obj.elems.size())
        throw std::invalid_argument("Cannot perform addition on vectors of different dimensionality");

    for (int i = 0; i < elems.size(); i++)
        elems[i] += obj.elems[i];
    return *this;
}

MATHCHIS::vector& MATHCHIS::vector::operator-=(const MATHCHIS::vector &obj) {
    if (elems.size() != obj.elems.size())
        throw std::invalid_argument("Cannot perform subtraction on vectors of different dimensionality");

    for (int i = 0; i < elems.size(); i++)
        elems[i] -= obj.elems[i];
    return *this;
}

MATHCHIS::vector& MATHCHIS::vector::operator*=(const float &obj) {
    for (float & elem : elems)
        elem *= obj;
    return *this;
}

MATHCHIS::vector& MATHCHIS::vector::operator/=(const float &obj) {
    for (float & elem : elems)
        elem /= obj;
    return *this;
}

// Static operators

float MATHCHIS::vector::dot(const MATHCHIS::vector &v1, const MATHCHIS::vector &v2) {
    if (v1.elems.size() != v2.elems.size())
        throw std::invalid_argument("Cannot perform dot product on vectors of different dimensionality");

    float dotProd = 0;

    for (int i = 0; i < v1.elems.size(); i++)
        dotProd += v1.elems[i] * v2.elems[i];

    return dotProd;
}

MATHCHIS::vector MATHCHIS::vector::cross(const MATHCHIS::vector &v1, const MATHCHIS::vector &v2) {
    if (v1.elems.size() != 3 || v2.elems.size() != 3)
        throw std::invalid_argument("Cannot perform cross product on non-3D vectors");

    std::vector<float> tElems;
    tElems.reserve(3);

    tElems.push_back(v1.elems[1]*v2.elems[2] - v1.elems[2]*v2.elems[1]);
    tElems.push_back(v1.elems[2]*v2.elems[0] - v1.elems[0]*v2.elems[2]);
    tElems.push_back(v1.elems[0]*v2.elems[1] - v1.elems[1]*v2.elems[0]);

    return vector(tElems);
}

float MATHCHIS::vector::mag2(const MATHCHIS::vector &vec) {
    float mg = 0;
    for (int i = 0; i < vec.elems.size(); i++)
        mg += vec.elems[i] * vec.elems[i];
    return mg;
}

float MATHCHIS::vector::mag(const MATHCHIS::vector &vec) {
    return sqrt(mag2(vec));
}

MATHCHIS::vector MATHCHIS::vector::normalized(const MATHCHIS::vector &vec) {
    std::vector<float> tElems;
    tElems.reserve(vec.elems.size());

    float mg = mag(vec);
    for (float elem : vec.elems)
        tElems.push_back(elem / mg);

    return vector(tElems);
}

int MATHCHIS::vector::normalize() {

    float mg = mag(*this);
    for (float & elem : elems)
        elem /= mg;

    return 0;
}

MATHCHIS::vector MATHCHIS::vector::reflect(MATHCHIS::vector &norm) {
    return (*this) - norm*vector::dot((*this), norm)*2;
}

MATHCHIS::vector MATHCHIS::vector::operator*(const MATHCHIS::vector &obj) const {
    std::vector<float> tElems;
    tElems.reserve(elems.size());

    for (int i = 0; i < elems.size(); i++)
        tElems.push_back(elems[i] * obj.elems[i]);

    return vector(tElems);
}

MATHCHIS::vector &MATHCHIS::vector::operator*=(const MATHCHIS::vector &obj) {
    (*this) = (*this)*obj;
    return *this;
}

// COLOR OPERATORS

int MATHCHIS::vector::col_tobytes() {

    float buf[3];
    for (int i = 0; i < 3; i++) {
        if (elems[i] > 1)
            buf[i] = 1;
        else if (elems[i] < 0)
            buf[i] = 0;
        else
            buf[i] = elems[i];
    }

    return (unsigned char)(buf[0]*255) |
           ((unsigned char)(buf[1]*255) << 8) |
           ((unsigned char)(buf[2]*255) << 16);
}

MATHCHIS::vector MATHCHIS::vector::operator*(const MATHCHIS::matrix &obj) const {
    if (obj.m != elems.size())
        throw std::invalid_argument("Cannot multiply matrices with incongruent size");

    std::vector<float> tElems;
    tElems.reserve(obj.n);

    for (int n_i = 0; n_i < obj.n; n_i++) {
        float net = 0;
        for (int m_i = 0; m_i < elems.size(); m_i++) {
            net += elems[m_i] * obj.elems[m_i * obj.n + n_i];
        }
        tElems.push_back(net);
    }
    return MATHCHIS::vector(tElems);
}

MATHCHIS::vector &MATHCHIS::vector::operator*=(const MATHCHIS::matrix &obj) {
    return *this;
}
