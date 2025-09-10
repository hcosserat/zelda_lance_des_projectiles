#include "Vector.h"
#include <cmath> // pour fmodf et sqrt

// Constructeur
Vector::Vector(float x, float y, float z)
	: x(x)
	, y(y)
	, z(z) { }

// Méthodes membres
glm::vec2 Vector::vec2() const {
	return glm::vec2(x, y);
}

glm::vec3 Vector::vec3() const {
	return glm::vec3(x, y, z);
}

ofColor Vector::color() const {
	return ofColor(x, y, z);
}

Vector Vector::operator+(const Vector & other) const {
	return Vector(x + other.x, y + other.y, z + other.z);
}

Vector Vector::operator*(float alpha) const {
	return Vector(alpha * x, alpha * y, alpha * z);
}

void Vector::operator+=(const Vector & other) {
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vector::operator*=(float alpha) {
	x *= alpha;
	y *= alpha;
	z *= alpha;
}

void Vector::operator%=(float a) {
	x = fmodf(x, a);
	y = fmodf(y, a);
	z = fmodf(z, a);
}

Vector Vector::operator-(const Vector & other) const {
	return Vector(x - other.x, y - other.y, z - other.z);
}

void Vector::operator-=(const Vector & other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

Vector Vector::operator*(const Vector & other) const {
	return Vector(x * other.x, y * other.y, z * other.z);
}

float Vector::normSquared() const {
	return x * x + y * y + z * z;
}

float Vector::norm() const {
	return sqrt(normSquared());
}

void Vector::normalize() {
	float n = norm();
	if (n > 0) {
		x /= n;
		y /= n;
		z /= n;
	}
}

Vector Vector::normalized() const {
	Vector result = *this;
	result.normalize();
	return result;
}

float Vector::dot(const Vector & other) const {
	return x * other.x + y * other.y + z * other.z;
}

Vector Vector::cross(const Vector & other) const {
	return Vector(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

float Vector::distance(const Vector & other) const {
	return (*this - other).norm();
}

float Vector::distanceSquared(const Vector & other) const {
	return (*this - other).normSquared();
}
