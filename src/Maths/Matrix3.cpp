#include "Matrix3.h"
#include <stdexcept>
#include <cmath>

// Constructeurs
Matrix3::Matrix3() : a(1, 0, 0), b(0, 1, 0), c(0, 0, 1) {
}

Matrix3::Matrix3(const Vector &col0, const Vector &col1, const Vector &col2) : a(col0), b(col1), c(col2) {
}

Matrix3 Matrix3::Identity() { return Matrix3(); }

float Matrix3::operator()(int r, int col) const {
    switch (col) {
        case 0: return (r == 0 ? a.x : (r == 1 ? a.y : a.z));
        case 1: return (r == 0 ? b.x : (r == 1 ? b.y : b.z));
        default: return (r == 0 ? c.x : (r == 1 ? c.y : c.z));
    }
}

float &Matrix3::operator()(int r, int col) {
    switch (col) {
        case 0: return (r == 0 ? a.x : (r == 1 ? a.y : a.z));
        case 1: return (r == 0 ? b.x : (r == 1 ? b.y : b.z));
        default: return (r == 0 ? c.x : (r == 1 ? c.y : c.z));
    }
}

// Opérations de base
Matrix3 Matrix3::operator+(const Matrix3 &M) const {
    return Matrix3(Vector(a.x + M.a.x, a.y + M.a.y, a.z + M.a.z),
                   Vector(b.x + M.b.x, b.y + M.b.y, b.z + M.b.z),
                   Vector(c.x + M.c.x, c.y + M.c.y, c.z + M.c.z));
}

Matrix3 Matrix3::operator-(const Matrix3 &M) const {
    return Matrix3(Vector(a.x - M.a.x, a.y - M.a.y, a.z - M.a.z),
                   Vector(b.x - M.b.x, b.y - M.b.y, b.z - M.b.z),
                   Vector(c.x - M.c.x, c.y - M.c.y, c.z - M.c.z));
}

Matrix3 Matrix3::operator*(float s) const {
    return Matrix3(Vector(a.x * s, a.y * s, a.z * s),
                   Vector(b.x * s, b.y * s, b.z * s),
                   Vector(c.x * s, c.y * s, c.z * s));
}

Matrix3 &Matrix3::operator*=(float s) {
    a.x *= s;
    a.y *= s;
    a.z *= s;
    b.x *= s;
    b.y *= s;
    b.z *= s;
    c.x *= s;
    c.y *= s;
    c.z *= s;
    return *this;
}

// Produits
Vector Matrix3::operator*(const Vector &v) const {
    // a*v.x + b*v.y + c*v.z
    return Vector(
        a.x * v.x + b.x * v.y + c.x * v.z,
        a.y * v.x + b.y * v.y + c.y * v.z,
        a.z * v.x + b.z * v.y + c.z * v.z
    );
}

Matrix3 Matrix3::operator*(const Matrix3 &M) const {
    // Résultat = [ (*this)*M.a  (*this)*M.b  (*this)*M.c ]
    return Matrix3((*this) * M.a, (*this) * M.b, (*this) * M.c);
}

// Transpose / det / inverse
Matrix3 Matrix3::transpose() const {
    return Matrix3(
        Vector(a.x, b.x, c.x),
        Vector(a.y, b.y, c.y),
        Vector(a.z, b.z, c.z)
    );
}

float Matrix3::determinant() const {
    return dot(a, cross(b, c)); // det([a b c]) = a · (b × c)
}

Matrix3 Matrix3::inverse(float eps) const {
	Vector c0 = cross(b, c);
	Vector c1 = cross(c, a);
	Vector c2 = cross(a, b);
	float det = dot(a, c0);
	if (std::fabs(det) < eps) throw std::runtime_error("Matrix3 non inversible");
	float invDet = 1.0f / det;
	return Matrix3(
		Vector(c0.x * invDet, c1.x * invDet, c2.x * invDet),
		Vector(c0.y * invDet, c1.y * invDet, c2.y * invDet),
		Vector(c0.z * invDet, c1.z * invDet, c2.z * invDet));
}

// Constructions
Matrix3 Matrix3::FromAxisAngle(const Vector &axis, float angleRad) {
    Vector u = normalize(axis);
    float c = std::cos(angleRad), s = std::sin(angleRad), t = 1.f - c;

    Vector col0(t * u.x * u.x + c, t * u.x * u.y + s * u.z, t * u.x * u.z - s * u.y);
    Vector col1(t * u.y * u.x - s * u.z, t * u.y * u.y + c, t * u.y * u.z + s * u.x);
    Vector col2(t * u.z * u.x + s * u.y, t * u.z * u.y - s * u.x, t * u.z * u.z + c);
    return Matrix3(col0, col1, col2);
}

Matrix3 Matrix3::FromEulerXYZ(float rx, float ry, float rz) {
    return FromAxisAngle(Vector(0, 0, 1), rz) * FromAxisAngle(Vector(0, 1, 0), ry) * FromAxisAngle(Vector(1, 0, 0), rx);
}

Matrix3 Matrix3::FromQuaternion(float w, float x, float y, float z) {
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    Vector col0(1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy));
    Vector col1(2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx));
    Vector col2(2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy));
    return Matrix3(col0, col1, col2);
}

// Orthonormalize
void Matrix3::Orthonormalize() {
    Vector x = normalize(a);
    float bx = dot(b, x);
    Vector y = Vector(b.x - bx * x.x, b.y - bx * x.y, b.z - bx * x.z);
    y = normalize(y);
    Vector z = cross(x, y);
    a = x;
    b = y;
    c = z;
}
