#pragma once

#include "../ofMain.h"
#include <ostream>
#include <cmath>

class Quaternion {

public:

	// Attributs
	float w, x, y, z;

	// Constructeur
	explicit Quaternion(float w = 1, float x = 0, float y = 0, float z = 0);

	glm::quat glmQuat() const;

	Quaternion neg();

	float norm() const;

	Quaternion conj() const;

	Quaternion inverse() const;

	Quaternion operator*(const Quaternion& other) const;

	Quaternion diff(const Quaternion& other) const;

	float dot(const Quaternion& other) const;

	Quaternion exp(float t) const;


}