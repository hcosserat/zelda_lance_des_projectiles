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

void Quaternion::normalize() {
	float n = norm();
	if (n > 0) {
		w /= n;
		x /= n;
		y /= n;
		z /= n;
	} else {
		w = 1;
		x = 0;
		y = 0;
		z = 0;
	}
}

Quaternion Quaternion::conj() const {
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inverse() const {
	float n = norm();
	if (n > 0) {
		Quaternion q = conj();
		q = q * (1.0f / (n * n));
		return q;
	} else {
		return Quaternion(0, 0, 0, 0); // Retourne un quaternion nul si la norme est z�ro
	}
}

Quaternion Quaternion::operator*(const Quaternion &other) const {
	return Quaternion(
		w * other.w - x * other.x - y * other.y - z * other.z,
		w * other.x + x * other.w + y * other.z - z * other.y,
		w * other.y - x * other.z + y * other.w + z * other.x,
		w * other.z + x * other.y - y * other.x + z * other.w);
}

Quaternion Quaternion::diff(const Quaternion &other) const {
	// D�placement angulaire de this vers other
	Quaternion conjThis = (*this).conj();
	return other * conjThis;
}

float Quaternion::dot(const Quaternion &other) const {
	return w * other.w + x * other.x + y * other.y + z * other.z;
}

Quaternion Quaternion::exp(float t) const {
	Quaternion q = *this;
	q.normalize();
	float angle = acos(q.w);
	float sinAngle = sin(angle);

	if (sinAngle < 1e-8f)
		return Quaternion(1, 0, 0, 0);

	float coeff = sin(t * angle) / sinAngle;

	return Quaternion(cos(t * angle),
		q.x * coeff,
		q.y * coeff,
		q.z * coeff);
}


void Quaternion::fromAxisAngle(const glm::vec3 &axis, float angleRad) {
	float halfAngle = angleRad / 2.0f;
	float sinHalfAngle = sin(halfAngle);
	w = cos(halfAngle);
	x = axis.x * sinHalfAngle;
	y = axis.y * sinHalfAngle;
	z = axis.z * sinHalfAngle;
}

Matrix4 Quaternion::toRotationMatrix4() const {
	Matrix4 R;
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;
	R(0, 0) = 1 - 2 * (yy + zz);
	R(0, 1) = 2 * (xy - wz);
	R(0, 2) = 2 * (xz + wy);
	R(0, 3) = 0;
	R(1, 0) = 2 * (xy + wz);
	R(1, 1) = 1 - 2 * (xx + zz);
	R(1, 2) = 2 * (yz - wx);
	R(1, 3) = 0;
	R(2, 0) = 2 * (xz - wy);
	R(2, 1) = 2 * (yz + wx);
	R(2, 2) = 1 - 2 * (xx + yy);
	R(2, 3) = 0;
	R(3, 0) = 0;
	R(3, 1) = 0;
	R(3, 2) = 0;
	R(3, 3) = 1;
	return R;
}

Matrix3 Quaternion::toRotationMatrix3() const {
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	// Colonne 0
	Vector c0(1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy));
	// Colonne 1
	Vector c1(2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx));
	// Colonne 2
	Vector c2(2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy));

	return Matrix3(c0, c1, c2);
}

glm::vec3 Quaternion::rotateVector(const glm::vec3 & v) const {
	Quaternion q = *this;
	q.normalize();

	Quaternion p(0, v.x, v.y, v.z);
	Quaternion result = q * p * q.conj();

	return glm::vec3(result.x, result.y, result.z);
}

Quaternion Quaternion::operator+(const Quaternion &other) const {
	return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
}

Quaternion Quaternion::operator*(float scalar) const {
	return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}
