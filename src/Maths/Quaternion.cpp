#include "Quaternion.h"

// Constructeur
Quaternion::Quaternion(float w, float x, float y, float z)
	: w(w)
	  , x(x)
	  , y(y)
	  , z(z) {
}

glm::quat Quaternion::glmQuat() const {
	return glm::quat(w, x, y, z);
}

Quaternion Quaternion::neg() {
	return Quaternion(-w, -x, -y, -z);
}

float Quaternion::norm() const {
	return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::conj() const {
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inverse() const {
	float n = norm();
	if (n > 0) {
		Quaternion conjQ = conj();
		return Quaternion(conjQ.w / (n * n), conjQ.x / (n * n), conjQ.y / (n * n), conjQ.z / (n * n));
	} else {
		return Quaternion(0, 0, 0, 0); // Retourne un quaternion nul si la norme est zéro
	}
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
	return Quaternion(
		w * other.w - x * other.x - y * other.y - z * other.z,
		w * other.x + x * other.w + y * other.z - z * other.y,
		w * other.y - x * other.z + y * other.w + z * other.x,
		w * other.z + x * other.y - y * other.x + z * other.w);
}

Quaternion Quaternion::diff(const Quaternion& other) const {	// Déplacement angulaire de this vers other
	Quaternion conjThis = (*this).conj();
	return other * conjThis;
}

float Quaternion::dot(const Quaternion& other) const {
	return w * other.w + x * other.x + y * other.y + z * other.z;
}

Quaternion Quaternion::exp(float t) const {
	float angle = acos(w);
	float sinAngle = sin(angle);
	if (sinAngle > 0) {
		float coeff = sin(t * angle) / sinAngle;
		return Quaternion(cos(t * angle), x * coeff, y * coeff, z * coeff);
	} else {
		return Quaternion(cos(t * angle), 0, 0, 0);
	}
}