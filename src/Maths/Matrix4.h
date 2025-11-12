#pragma once

#include <stdexcept>
#include "Vector.h"
#include "Matrix3.h"

class Matrix4 {
public:
    // Colonnes
    Vector c0, c1, c2, c3;

    // Constructeurs
    Matrix4();

    Matrix4(const Vector &col0, const Vector &col1, const Vector &col2, const Vector &col3);

    static Matrix4 Identity();

    float operator()(int r, int c) const;

    float &operator()(int r, int c);

    // Produits
    Matrix4 operator*(const Matrix4 &M) const;

    Matrix4 operator*(float s) const;

    Vector TransformPoint(const Vector &p) const;

    Vector TransformVector(const Vector &v) const;

    // Constructions
    static Matrix4 FromRotationTranslation(const Matrix3 &R, const Vector &t);

    static Matrix4 MakeTranslation(const Vector &t);

    static Matrix4 MakeScale(const Vector &s);

    static Matrix4 MakeRotationAxisAngle(const Vector &axis, float angleRad);

    static Matrix4 MakeRotationEulerXYZ(float rx, float ry, float rz);

    static Matrix4 MakeRotationQuaternion(float w, float x, float y, float z);

    static Matrix4 MakeTRS(const Vector &t, const Matrix3 &R, const Vector &s = Vector(1, 1, 1));

    // Inverse rapide pour transformation **rigide** (R orthonormale)
    Matrix4 InverseRigid() const;

    // Utilitaires
    Matrix3 RotationPart() const;

    Vector Translation() const;

private:
    static inline float dot(const Vector &u, const Vector &v) { return u.x * v.x + u.y * v.y + u.z * v.z; }
};
