#pragma once

#include "RigidBody.h"

class Box final : public RigidBody {
public:
	Vector dimensions; // Largeur, Hauteur, Profondeur

	// Constructeur
	Box(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
	    const Quaternion &orientation, const Vector &angularVel, const Vector &angularAcc, float mass,
	    const Matrix3 &invInertiaTensor, const Vector &dimensions);

	void drawShape() const;

	std::vector<Vector> getVertices() const {
		std::vector<Vector> vertices;
		float halfX = dimensions.x / 2.0f;
		float halfY = dimensions.y / 2.0f;
		float halfZ = dimensions.z / 2.0f;
		vertices.emplace_back(halfX, halfY, halfZ);
		vertices.emplace_back(-halfX, halfY, halfZ);
		vertices.emplace_back(halfX, -halfY, halfZ);
		vertices.emplace_back(-halfX, -halfY, halfZ);
		vertices.emplace_back(halfX, halfY, -halfZ);
		vertices.emplace_back(-halfX, halfY, -halfZ);
		vertices.emplace_back(halfX, -halfY, -halfZ);
		vertices.emplace_back(-halfX, -halfY, -halfZ);

		for (auto &vertice: vertices) {
			glm::vec3 inputVec(vertice.x, vertice.y, vertice.z);
			glm::vec3 result = orientation.rotateVector(inputVec);
			Vector rotatedVertex(result.x, result.y, result.z);
			vertice = rotatedVertex + center;
		}
		return vertices;
	}
};
