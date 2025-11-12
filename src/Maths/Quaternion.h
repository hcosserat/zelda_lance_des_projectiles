#pragma once

#include "../ofMain.h"
#include <ostream>
#include <cmath>
#include "Matrix4.h"
#include "glm/vec3.hpp"

class Quaternion {
public:
    // Attributs
    float w, x, y, z;

    // Constructeur
    explicit Quaternion(float w = 1, float x = 0, float y = 0, float z = 0);

    glm::quat glmQuat() const;

    Quaternion neg();

    float norm() const;

    void normalize();

    Quaternion conj() const;

    Quaternion inverse() const;

    Quaternion operator*(const Quaternion &other) const;

    Quaternion diff(const Quaternion &other) const;

    float dot(const Quaternion &other) const;

    Quaternion exp(float t) const;

    void fromAxisAngle(const glm::vec3 &axis, float angleRad);

    Matrix4 toRotationMatrix4() const;

    glm::vec3 rotateVector(const glm::vec3 &v) const;

    Quaternion operator+(const Quaternion &other) const;

    Quaternion operator*(float scalar) const;
};
