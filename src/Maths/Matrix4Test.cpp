#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Matrix4.h"
#include "Matrix4Test.h"

static bool approx(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) < eps;
}

static void test_constructor_identity() {
    Matrix4 I = Matrix4::Identity();

    assert(approx(I(0, 0), 1)); assert(approx(I(1, 0), 0)); assert(approx(I(2, 0), 0));
    assert(approx(I(0, 1), 0)); assert(approx(I(1, 1), 1)); assert(approx(I(2, 1), 0));
    assert(approx(I(0, 2), 0)); assert(approx(I(1, 2), 0)); assert(approx(I(2, 2), 1));

    assert(approx(I(0, 3), 0));
    assert(approx(I(1, 3), 0));
    assert(approx(I(2, 3), 0));
}

static void test_access_read_write() {
    Matrix4 M;

    M(0, 0) = 2;  M(1, 1) = 3;  M(2, 2) = 4;
    M(0, 3) = 10; M(1, 3) = 20; M(2, 3) = 30;

    assert(approx(M(0, 0), 2));
    assert(approx(M(1, 1), 3));
    assert(approx(M(2, 2), 4));

    assert(approx(M(0, 3), 10));
    assert(approx(M(1, 3), 20));
    assert(approx(M(2, 3), 30));
}

static void test_mul_scalar() {
    Matrix4 I = Matrix4::Identity();
    Matrix4 M = I * 2.f;

    assert(approx(M(0, 0), 2)); assert(approx(M(1, 1), 2)); assert(approx(M(2, 2), 2));
    assert(approx(M(0, 3), 0)); assert(approx(M(1, 3), 0)); assert(approx(M(2, 3), 0));
}

static void test_mul_matrix() {
    Matrix4 A = Matrix4::MakeTranslation(Vector(1, 2, 3));
    Matrix4 B = Matrix4::MakeTranslation(Vector(4, 5, 6));
    Matrix4 C = A * B;

    Vector t = C.Translation();
    assert(approx(t.x, 1 + 4));
    assert(approx(t.y, 2 + 5));
    assert(approx(t.z, 3 + 6));
}

static void test_transform_point() {
    Matrix4 T = Matrix4::MakeTranslation(Vector(1, 2, 3));

    Vector p = T.TransformPoint(Vector(10, 20, 30));

    assert(approx(p.x, 11));
    assert(approx(p.y, 22));
    assert(approx(p.z, 33));
}

static void test_transform_vector() {
    Matrix4 S = Matrix4::MakeScale(Vector(2, 3, 4));

    Vector v = S.TransformVector(Vector(1, 1, 1));

    assert(approx(v.x, 2));
    assert(approx(v.y, 3));
    assert(approx(v.z, 4));
}

static void test_make_translation() {
    Matrix4 T = Matrix4::MakeTranslation(Vector(7, 8, 9));

    Vector t = T.Translation();
    assert(approx(t.x, 7));
    assert(approx(t.y, 8));
    assert(approx(t.z, 9));
}

static void test_make_scale() {
    Matrix4 S = Matrix4::MakeScale(Vector(2, 4, 6));

    assert(approx(S(0, 0), 2));
    assert(approx(S(1, 1), 4));
    assert(approx(S(2, 2), 6));
}

static void test_rotation_axis_angle() {
    Matrix4 R = Matrix4::MakeRotationAxisAngle(Vector(0, 0, 1), M_PI / 2);

    Vector x = R.TransformVector(Vector(1, 0, 0));

    assert(approx(x.x, 0));
    assert(approx(x.y, 1));
}

static void test_euler_xyz() {
    Matrix4 R = Matrix4::MakeRotationEulerXYZ(0, 0, M_PI / 2);

    Vector x = R.TransformVector(Vector(1, 0, 0));

    assert(approx(x.x, 0));
    assert(approx(x.y, 1));
}

static void test_quaternion() {
    Matrix4 R = Matrix4::MakeRotationQuaternion(std::cos(M_PI / 4), 0, 0, std::sin(M_PI / 4));

    Vector x = R.TransformVector(Vector(1, 0, 0));

    assert(approx(x.x, 0));
    assert(approx(x.y, 1));
}

static void test_trs() {
    Matrix4 M = Matrix4::MakeTRS(
        Vector(1, 2, 3),
        Matrix3::Identity(),
        Vector(2, 3, 4)
    );

    assert(approx(M(0, 0), 2));
    assert(approx(M(1, 1), 3));
    assert(approx(M(2, 2), 4));

    Vector t = M.Translation();
    assert(approx(t.x, 1));
    assert(approx(t.y, 2));
    assert(approx(t.z, 3));
}

static void test_inverse_rigid() {
    Matrix4 T = Matrix4::MakeTranslation(Vector(1, 2, 3));
    Matrix4 R = Matrix4::MakeRotationAxisAngle(Vector(0, 0, 1), M_PI / 2);

    Matrix4 M = R * T;
    Matrix4 inv = M.InverseRigid();

    Matrix4 shouldBeId = M * inv;

    assert(approx(shouldBeId(0, 0), 1));
    assert(approx(shouldBeId(1, 1), 1));
    assert(approx(shouldBeId(2, 2), 1));

    assert(std::fabs(shouldBeId(0, 3)) < 1e-4);
    assert(std::fabs(shouldBeId(1, 3)) < 1e-4);
    assert(std::fabs(shouldBeId(2, 3)) < 1e-4);
}

static void test_rotation_and_translation_getters() {
    Matrix4 M = Matrix4::MakeTranslation(Vector(4, 5, 6));

    Vector t = M.Translation();
    assert(approx(t.x, 4));
    assert(approx(t.y, 5));
    assert(approx(t.z, 6));

    Matrix3 R = M.RotationPart();
    assert(approx(R.a.x, 1));
    assert(approx(R.b.y, 1));
    assert(approx(R.c.z, 1));
}

int test_matrix4() {
    test_constructor_identity();
    test_access_read_write();
    test_mul_scalar();
    test_mul_matrix();
    test_transform_point();
    test_transform_vector();
    test_make_translation();
    test_make_scale();
    test_rotation_axis_angle();
    test_euler_xyz();
    test_quaternion();
    test_trs();
    test_inverse_rigid();
    test_rotation_and_translation_getters();

	std::cout << "Tests Matrix4 OK !" << std::endl;
    return 0;
}
