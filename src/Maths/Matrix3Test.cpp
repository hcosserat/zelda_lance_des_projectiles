#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "Matrix3Test.h"
#include "Matrix3.h"

void test_matrix3_constructor() {
    Matrix3 I;
    assert(I.a.x == 1 && I.b.y == 1 && I.c.z == 1);
    assert(I.a.y == 0 && I.a.z == 0);
    assert(I.b.x == 0 && I.b.z == 0);
    assert(I.c.x == 0 && I.c.y == 0);

    Matrix3 M(Vector(1, 2, 3), Vector(4, 5, 6), Vector(7, 8, 9));
    assert(M.a.x == 1 && M.b.y == 5 && M.c.z == 9);
}

void test_matrix3_access() {
    Matrix3 M(Vector(1, 2, 3), Vector(4, 5, 6), Vector(7, 8, 9));
    assert(M(0, 0) == 1);
    assert(M(1, 1) == 5);
    assert(M(2, 2) == 9);

    M(1, 2) = 42;
    assert(M.c.y == 42);
}

void test_matrix3_add_sub() {
    Matrix3 A(Vector(1, 1, 1), Vector(2, 2, 2), Vector(3, 3, 3));
    Matrix3 B(Vector(1, 2, 3), Vector(4, 5, 6), Vector(7, 8, 9));

    Matrix3 C = A + B;
    assert(C.a.x == 2 && C.c.z == 12);

    Matrix3 D = B - A;
    assert(D.a.x == 0 && D.c.z == 6);
}

void test_matrix3_scalar_mul() {
    Matrix3 M(Vector(1, 2, 3), Vector(4, 5, 6), Vector(7, 8, 9));
    Matrix3 R = M * 2;
    assert(R.a.x == 2 && R.b.y == 10 && R.c.z == 18);

    M *= 2;
    assert(M.a.x == 2 && M.b.y == 10 && M.c.z == 18);
}

void test_matrix3_vector_mul() {
    Matrix3 M(Vector(1, 0, 0), Vector(0, 2, 0), Vector(0, 0, 3));
    Vector v(1, 2, 3);
    Vector r = M * v;
    assert(r.x == 1);
    assert(r.y == 4);
    assert(r.z == 9);
}

void test_matrix3_matrix_mul() {
    Matrix3 A(Vector(1, 0, 0), Vector(0, 2, 0), Vector(0, 0, 3));
    Matrix3 B(Vector(1, 2, 3), Vector(1, 1, 1), Vector(2, 2, 2));

    Matrix3 C = A * B;

    // V�rification colonnes
    assert(C.a.x == 1 && C.a.y == 4 && C.a.z == 9);
    assert(C.b.x == 1 && C.b.y == 2 && C.b.z == 3);
    assert(C.c.x == 2 && C.c.y == 4 && C.c.z == 6);
}


void test_matrix3_transpose() {
    Matrix3 M(Vector(1, 2, 3), Vector(4, 5, 6), Vector(7, 8, 9));
    Matrix3 T = M.transpose();

    assert(T.a.x == 1 && T.b.x == 2 && T.c.x == 3);
    assert(T.a.y == 4 && T.b.y == 5 && T.c.y == 6);
}

void test_matrix3_determinant() {
    Matrix3 M(Vector(1, 2, 3), Vector(0, 1, 4), Vector(5, 6, 0));
    float det = M.determinant();
    assert(std::fabs(det - 1.0f) < 1e-6f);
}

void test_matrix3_inverse() {
    Matrix3 M(Vector(1, 2, 3), Vector(0, 1, 4), Vector(5, 6, 0));

    Matrix3 Inv = M.inverse();

    Matrix3 I = M * Inv;

    // Check ? Identity
    assert(std::fabs(I(0, 0) - 1) < 1e-5);
    assert(std::fabs(I(1, 1) - 1) < 1e-5);
    assert(std::fabs(I(2, 2) - 1) < 1e-5);

    assert(std::fabs(I(0, 1)) < 1e-5);
    assert(std::fabs(I(0, 2)) < 1e-5);
    assert(std::fabs(I(1, 0)) < 1e-5);
}

void test_matrix3_from_axis_angle() {
    Matrix3 R = Matrix3::FromAxisAngle(Vector(0, 0, 1), M_PI / 2);
    Vector v(1, 0, 0);
    Vector r = R * v;

    assert(std::fabs(r.x) < 1e-5);
    assert(std::fabs(r.y - 1) < 1e-5);
}

void test_matrix3_from_euler() {
    Matrix3 R = Matrix3::FromEulerXYZ(0, 0, M_PI / 2);
    Vector r = R * Vector(1, 0, 0);

    assert(std::fabs(r.x) < 1e-5);
    assert(std::fabs(r.y - 1) < 1e-5);
}

void test_matrix3_from_quaternion() {
    float s = std::sqrt(0.5f);
    Matrix3 R = Matrix3::FromQuaternion(s, 0, 0, s); // 90� autour de Z

    Vector r = R * Vector(1, 0, 0);
    assert(std::fabs(r.x) < 1e-5);
    assert(std::fabs(r.y - 1) < 1e-5);
}

void test_matrix3_orthonormalize() {
    Matrix3 M(Vector(1, 0, 0), Vector(1, 1, 0), Vector(0, 0, 1));
    M.Orthonormalize();

    assert(std::fabs(M.a.dot(M.b)) < 1e-5);
    assert(std::fabs(M.a.dot(M.c)) < 1e-5);
    assert(std::fabs(M.b.dot(M.c)) < 1e-5);

    assert(std::fabs(M.a.norm() - 1) < 1e-5);
    assert(std::fabs(M.b.norm() - 1) < 1e-5);
    assert(std::fabs(M.c.norm() - 1) < 1e-5);
}

int test_matrix3() {
    test_matrix3_constructor();
    test_matrix3_access();
    test_matrix3_add_sub();
    test_matrix3_scalar_mul();
    test_matrix3_vector_mul();
    test_matrix3_matrix_mul();
    test_matrix3_transpose();
    test_matrix3_determinant();
    test_matrix3_inverse();
    test_matrix3_from_axis_angle();
    test_matrix3_from_euler();
    test_matrix3_from_quaternion();
    test_matrix3_orthonormalize();

    std::cout << "Tests Matrix3 OK !" << std::endl;
    return 0;
}
