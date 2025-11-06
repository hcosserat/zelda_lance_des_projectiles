#pragma once

#include <stdexcept>
#include <cmath>
#include "Vector.h"

class Matrix3 {
public:
    Vector a, b, c;

    // Constructeurs
    Matrix3();
    Matrix3(const Vector& col0, const Vector& col1, const Vector& col2);

    static Matrix3 Identity();
    static Matrix3 FromAxisAngle(const Vector& axis, float angleRad);
    static Matrix3 FromEulerXYZ(float rx, float ry, float rz);
    static Matrix3 FromQuaternion(float w, float x, float y, float z);

    float  operator()(int r, int c) const;
    float& operator()(int r, int c);

    // Opérations de base
    Matrix3 operator+(const Matrix3& M) const;
    Matrix3 operator-(const Matrix3& M) const;
    Matrix3 operator*(float s) const;
    Matrix3& operator*=(float s);

    // Produits
    Vector  operator*(const Vector& v) const;   // M * v (v colonne)
    Matrix3 operator*(const Matrix3& M) const;  // M * N

    // Transpose / det / inverse
    Matrix3 transpose() const;
    float   determinant() const;
    Matrix3 inverse(float eps = 1e-8f) const;

    // Orthonormalisation
    void Orthonormalize();

private:

    static inline float  dot(const Vector& u, const Vector& v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
    static inline Vector cross(const Vector& u, const Vector& v) {
        return Vector(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
    }
    static inline float  norm(const Vector& v) { return std::sqrt(dot(v,v)); }
    static inline Vector normalize(const Vector& v) {
        float n = norm(v); return (n > 0.f) ? Vector(v.x/n, v.y/n, v.z/n) : Vector(0,0,0);
    }
};
