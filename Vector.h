#pragma once

#include "ofMain.h"

class Vector {
public:
    float x, y, z;

    // Constructor: initializes vector components (default 0)
    explicit Vector(const float x = 0, const float y = 0, const float z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Returns a glm::vec2 with x and y components
    glm::vec2 vec2() const {
        return glm::vec2(x, y);
    }

    // Returns a glm::vec3 with x, y, z components
    glm::vec3 vec3() const {
        return glm::vec3(x, y, z);
    }

    // Returns an ofColor constructed from x, y, z
    ofColor color() const {
        return ofColor(x, y, z);
    }

    // Vector addition
    Vector operator+(const Vector &other) const {
        return Vector(x + other.x, y + other.y, z + other.z);
    }

    // Scalar multiplication
    Vector operator*(const float alpha) const {
        return Vector(alpha * x, alpha * y, alpha * z);
    }

    // In-place vector addition
    void operator+=(const Vector &other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    // In-place scalar multiplication
    void operator*=(const float alpha) {
        x *= alpha;
        y *= alpha;
        z *= alpha;
    }

    // In-place component-wise modulo operation
    void operator%=(const float a) {
        x = fmodf(x, a);
        y = fmodf(y, a);
        z = fmodf(z, a);
    }

    // Vector subtraction
    Vector operator-(const Vector &other) const {
        return Vector(x - other.x, y - other.y, z - other.z);
    }

    // In-place vector subtraction
    void operator-=(const Vector &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    // Component-wise multiplication
    Vector operator*(const Vector &other) const {
        return Vector(x * other.x, y * other.y, z * other.z);
    }

    // Returns the squared norm (length squared) of the vector
    float normSquared() const {
        return x * x + y * y + z * z;
    }

    // Returns the norm (magnitude) of the vector
    float norm() const {
        return sqrt(normSquared());
    }

    // Normalizes the vector in-place
    void normalize() {
        float n = norm();
        if (n > 0) {
            x /= n;
            y /= n;
            z /= n;
        }
    }

    // Returns a normalized copy of the vector
    Vector normalized() const {
        Vector result = *this;
        result.normalize();
        return result;
    }

    // Returns the dot product with another vector
    float dot(const Vector &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Returns the cross product with another vector
    Vector cross(const Vector &other) const {
        return Vector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Returns the Euclidean distance to another vector
    float distance(const Vector &other) const {
        return (*this - other).norm();
    }

    // Returns the squared distance to another vector
    float distanceSquared(const Vector &other) const {
        return (*this - other).normSquared();
    }
};
