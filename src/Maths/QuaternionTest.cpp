#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Quaternion.h"
#include "QuaternionTest.h"

static bool approx(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) < eps;
}

static void test_constructor() {
    Quaternion q;
    assert(approx(q.w, 1));
    assert(approx(q.x, 0));
    assert(approx(q.y, 0));
    assert(approx(q.z, 0));
}

static void test_norm_normalize() {
    Quaternion q(0, 3, 4, 0);
    assert(approx(q.norm(), 5));

    q.normalize();
    assert(approx(q.norm(), 1));
    assert(approx(q.x, 3.0f / 5.0f));
    assert(approx(q.y, 4.0f / 5.0f));
}

static void test_conjugate_inverse() {
    Quaternion q(1, 2, 3, 4);
    Quaternion c = q.conj();

    assert(approx(c.w, 1));
    assert(approx(c.x, -2));
    assert(approx(c.y, -3));
    assert(approx(c.z, -4));

    Quaternion inv = q.inverse();
    Quaternion id = q * inv;

    // Should be approximately identity quaternion
    assert(approx(id.w, 1));
    assert(std::fabs(id.x) < 1e-4);
    assert(std::fabs(id.y) < 1e-4);
    assert(std::fabs(id.z) < 1e-4);
}

static void test_mul_quaternion() {
    Quaternion q1;
    Quaternion q2(0, 1, 0, 0);

    Quaternion r = q1 * q2;

    assert(approx(r.w, q2.w));
    assert(approx(r.x, q2.x));
}

static void test_dot() {
    Quaternion q1(1, 2, 3, 4);
    Quaternion q2(5, 6, 7, 8);

    float d = q1.dot(q2);
    assert(approx(d, 1 * 5 + 2 * 6 + 3 * 7 + 4 * 8));
}

static void test_neg() {
    Quaternion q(1, -2, 3, -4);
    Quaternion n = q.neg();

    assert(approx(n.w, -1));
    assert(approx(n.x, 2));
    assert(approx(n.y, -3));
    assert(approx(n.z, 4));
}

static void test_axis_angle() {
    Quaternion q;
    q.fromAxisAngle(glm::vec3(0, 0, 1), M_PI / 2);

    // Rotating forward vector should turn left
    glm::vec3 v = q.rotateVector(glm::vec3(1, 0, 0));

    assert(approx(v.x, 0));
    assert(approx(v.y, 1));
}

static void test_rotation_matrix() {
    Quaternion q;
    q.fromAxisAngle(glm::vec3(0, 0, 1), M_PI / 2);

    Matrix4 R = q.toRotationMatrix4();

    // Same expected rotation as Euler Z 90°
    assert(approx(R(0, 0), 0));
    assert(approx(R(1, 0), 1));
    assert(approx(R(0, 1), -1));
    assert(approx(R(1, 1), 0));
}

static void test_rotate_vector() {
    Quaternion q;
    q.fromAxisAngle(glm::vec3(0, 1, 0), M_PI / 2);

    glm::vec3 v = q.rotateVector(glm::vec3(1, 0, 0));

    assert(approx(v.x, 0));
    assert(approx(v.z, -1));
}

static void test_add_scalar_mul() {
    Quaternion q1(1, 2, 3, 4);
    Quaternion q2(5, 6, 7, 8);

    Quaternion s = q1 + q2;
    assert(approx(s.w, 6));
    assert(approx(s.x, 8));
    assert(approx(s.y, 10));
    assert(approx(s.z, 12));

    Quaternion m = q1 * 2.f;
    assert(approx(m.w, 2));
    assert(approx(m.x, 4));
    assert(approx(m.y, 6));
    assert(approx(m.z, 8));
}

static void test_exp() {
    Quaternion q;
    q.fromAxisAngle(glm::vec3(0, 0, 1), M_PI / 2);

    Quaternion half = q.exp(0.5f);
    glm::vec3 v = half.rotateVector(glm::vec3(1, 0, 0));

    // 45° rotation
    assert(approx(v.x, std::sqrt(0.5f)));
    assert(approx(v.y, std::sqrt(0.5f)));
}

static void test_diff() {
    Quaternion q1;
    Quaternion q2;
    q2.fromAxisAngle(glm::vec3(0, 0, 1), M_PI / 2);

    Quaternion d = q1.diff(q2); // d = q2 * q1^-1 = q2

    assert(approx(d.w, q2.w));
    assert(approx(d.x, q2.x));
    assert(approx(d.y, q2.y));
    assert(approx(d.z, q2.z));
}

int test_quaternion() {
    test_constructor();
    test_norm_normalize();
    test_conjugate_inverse();
    test_mul_quaternion();
    test_dot();
    test_neg();
    test_axis_angle();
    test_rotation_matrix();
    test_rotate_vector();
    test_add_scalar_mul();
    test_exp();
    test_diff();

	std::cout << "Tests Quaternions OK !" << std::endl;
    return 0;
}
