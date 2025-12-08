#include "NarrowPhase.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <glm/glm.hpp>

CollisionData NarrowPhase::CheckCollision(RigidBody* body1, RigidBody* body2) {
	if (body1->shape == BOX && body2->shape == PLANE) {
		return BoxAndPlane(static_cast<Box*>(body1), static_cast<Plane*>(body2));
	}
	else if (body1->shape == PLANE && body2->shape == BOX) {
		return BoxAndPlane(static_cast<Box*>(body2), static_cast<Plane*>(body1));
	}
	else if (body1->shape == BOX && body2->shape == BOX) {
		return BoxAndBox(static_cast<Box*>(body1), static_cast<Box*>(body2));
	}
	return CollisionData();
}

CollisionData NarrowPhase::BoxAndPlane(Box* box, Plane* plane) {
	CollisionData collisionData;
	auto vertices = box->getVertices();
	for (const auto& Q : vertices) {
		Vector boxToPlane = Q - plane->point; // Q - P
		float distance = boxToPlane.dot(plane->normal); // t = (Q - P) · n
		if (distance < 0) {
			// Collision détectée
			collisionData.addContact(Q - plane->normal * distance, plane->normal, -distance);
		}
	}
	return collisionData;
}

CollisionData NarrowPhase::BoxAndBox(Box* box1, Box* box2) {
	CollisionData collisionData;
	TestVerticesAgainstBox(box1, box2, collisionData);
	TestVerticesAgainstBox(box2, box1, collisionData);
	return collisionData;
}

void NarrowPhase::TestVerticesAgainstBox(Box* box1, Box* box2, CollisionData& collisionData) {
    auto vertices = box1->getVertices();
	// Récupération des axes locaux de box2 (glm car Quaternion utilise glm)
    glm::vec3 glmAxisX(1.0f, 0.0f, 0.0f);
    glm::vec3 glmAxisY(0.0f, 1.0f, 0.0f);
    glm::vec3 glmAxisZ(0.0f, 0.0f, 1.0f);
    glm::vec3 rawX = box2->orientation.rotateVector(glmAxisX);
    glm::vec3 rawY = box2->orientation.rotateVector(glmAxisY);
    glm::vec3 rawZ = box2->orientation.rotateVector(glmAxisZ);
    Vector axisX(rawX.x, rawX.y, rawX.z);
    Vector axisY(rawY.x, rawY.y, rawY.z);
    Vector axisZ(rawZ.x, rawZ.y, rawZ.z);
    Vector axes[3] = { axisX, axisY, axisZ };
	// Demi-dimensions de box2
    float halfDims[3] = {
        box2->dimensions.x / 2.0f,
        box2->dimensions.y / 2.0f,
        box2->dimensions.z / 2.0f
    };

    for (const auto& Q : vertices) {
        Vector relPos = Q - box2->center;
        bool isInside = true;
        float minPenetration = std::numeric_limits<float>::max();
        Vector bestNormal;
		// Test de chaque axe
        for (int i = 0; i < 3; i++) {
            float dist = relPos.dot(axes[i]);

            float distToPosFace = halfDims[i] - dist;
            float distToNegFace = halfDims[i] + dist;

            if (distToPosFace < 0 || distToNegFace < 0) {
                isInside = false;
                break;
            }

            if (distToPosFace < minPenetration) {
                minPenetration = distToPosFace;
                bestNormal = axes[i];
            }
            if (distToNegFace < minPenetration) {
                minPenetration = distToNegFace;
                bestNormal = axes[i] * -1.0f;
            }
        }

        if (isInside) {
            collisionData.addContact(Q, bestNormal, minPenetration);
        }
    }
}