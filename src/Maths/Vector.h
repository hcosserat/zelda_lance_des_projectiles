#pragma once

#include "../ofMain.h"

class Vector {
public:
    // Attributs
    float x, y, z;

    // Constructeur
    explicit Vector(float x = 0, float y = 0, float z = 0);

    glm::vec2 vec2() const;

    glm::vec3 vec3() const;

    ofColor color() const;

    Vector operator+(const Vector &other) const;

    Vector operator*(float alpha) const;

    void operator+=(const Vector &other);

    void operator*=(float alpha);

    void operator%=(float a);

    Vector operator-(const Vector &other) const;

    void operator-=(const Vector &other);

    Vector operator*(const Vector &other) const;

    bool operator==(const Vector &other) const;

    // Checks if x, y and z are all NAN
    bool isNAN() const;

    float normSquared() const;

    float norm() const;

    void normalize();

    Vector normalized() const;

    float dot(const Vector &other) const;

    Vector cross(const Vector &other) const;

    float distance(const Vector &other) const;

    float distanceSquared(const Vector &other) const;
};

Vector operator*(float alpha, const Vector &vec);
