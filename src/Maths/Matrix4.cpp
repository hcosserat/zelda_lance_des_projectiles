#include "Matrix4.h"
#include <cmath>
#include <stdexcept>

// Constructeurs
Matrix4::Matrix4() : c0(1, 0, 0), c1(0, 1, 0), c2(0, 0, 1), c3(0, 0, 0) {
}

Matrix4::Matrix4(const Vector &col0, const Vector &col1, const Vector &col2, const Vector &col3)
    : c0(col0), c1(col1), c2(col2), c3(col3) {
}

Matrix4 Matrix4::Identity() { return Matrix4(); }

// Accès
float Matrix4::operator()(int r, int c) const {
    const Vector *cols[4] = {&c0, &c1, &c2, &c3};
    if (c >= 0 && c < 4) {
        if (r >= 0 && r < 3) {
            const Vector &col = *cols[c];
            return (r == 0 ? col.x : (r == 1 ? col.y : col.z));
        } else if (r == 3) {
            return (c == 3) ? 1.f : 0.f;
        }
    }
    return 0.f;
}

float &Matrix4::operator()(int r, int c) {
    static float dummy = 0.f;
    if (c >= 0 && c < 4 && r >= 0 && r < 3) {
        Vector *cols[4] = {&c0, &c1, &c2, &c3};
        Vector &col = *cols[c];
        return (r == 0 ? col.x : (r == 1 ? col.y : col.z));
    }
    return dummy;
}

// Produits
Matrix4 Matrix4::operator*(const Matrix4 &M) const {
    Matrix3 R1(c0, c1, c2);
    Matrix3 R2(M.c0, M.c1, M.c2);

    Matrix3 R = R1 * R2;
    Vector t = TransformPoint(M.c3); // R1 * t2 + t1

    return Matrix4(R.a, R.b, R.c, t);
}

Matrix4 Matrix4::operator*(float s) const {
    return Matrix4(Vector(c0.x * s, c0.y * s, c0.z * s),
                   Vector(c1.x * s, c1.y * s, c1.z * s),
                   Vector(c2.x * s, c2.y * s, c2.z * s),
                   Vector(c3.x * s, c3.y * s, c3.z * s));
}

Vector Matrix4::TransformPoint(const Vector &p) const {
    // R * p + t
    return Vector(
        c0.x * p.x + c1.x * p.y + c2.x * p.z + c3.x,
        c0.y * p.x + c1.y * p.y + c2.y * p.z + c3.y,
        c0.z * p.x + c1.z * p.y + c2.z * p.z + c3.z
    );
}

Vector Matrix4::TransformVector(const Vector &v) const {
    // R * v
    return Vector(
        c0.x * v.x + c1.x * v.y + c2.x * v.z,
        c0.y * v.x + c1.y * v.y + c2.y * v.z,
        c0.z * v.x + c1.z * v.y + c2.z * v.z
    );
}

// Constructions
Matrix4 Matrix4::FromRotationTranslation(const Matrix3 &R, const Vector &t) {
    return Matrix4(R.a, R.b, R.c, t);
}

Matrix4 Matrix4::MakeTranslation(const Vector &t) {
    return FromRotationTranslation(Matrix3::Identity(), t);
}

Matrix4 Matrix4::MakeScale(const Vector &s) {
    return Matrix4(Vector(s.x, 0, 0), Vector(0, s.y, 0), Vector(0, 0, s.z), Vector(0, 0, 0));
}

Matrix4 Matrix4::MakeRotationAxisAngle(const Vector &axis, float angleRad) {
    return FromRotationTranslation(Matrix3::FromAxisAngle(axis, angleRad), Vector(0, 0, 0));
}

Matrix4 Matrix4::MakeRotationEulerXYZ(float rx, float ry, float rz) {
    return FromRotationTranslation(Matrix3::FromEulerXYZ(rx, ry, rz), Vector(0, 0, 0));
}

Matrix4 Matrix4::MakeRotationQuaternion(float w, float x, float y, float z) {
    return FromRotationTranslation(Matrix3::FromQuaternion(w, x, y, z), Vector(0, 0, 0));
}

Matrix4 Matrix4::MakeTRS(const Vector &t, const Matrix3 &R, const Vector &s) {
    Matrix3 RS(Vector(R.a.x * s.x, R.a.y * s.x, R.a.z * s.x),
               Vector(R.b.x * s.y, R.b.y * s.y, R.b.z * s.y),
               Vector(R.c.x * s.z, R.c.y * s.z, R.c.z * s.z));
    return FromRotationTranslation(RS, t);
}

// Inverse rigide
Matrix4 Matrix4::InverseRigid() const {
    Matrix3 R(c0, c1, c2);
    Matrix3 Rt = R.transpose();
    Vector t = c3;
    Vector t_inv(
        -(Rt.a.x * t.x + Rt.b.x * t.y + Rt.c.x * t.z),
        -(Rt.a.y * t.x + Rt.b.y * t.y + Rt.c.y * t.z),
        -(Rt.a.z * t.x + Rt.b.z * t.y + Rt.c.z * t.z)
    );
    return FromRotationTranslation(Rt, t_inv);
}

// Utilitaires
Matrix3 Matrix4::RotationPart() const { return Matrix3(c0, c1, c2); }
Vector Matrix4::Translation() const { return c3; }
