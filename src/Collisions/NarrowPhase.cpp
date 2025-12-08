#include "NarrowPhase.h"
#include <limits>

CollisionData NarrowPhase::CheckCollision(RigidBody *body1, RigidBody *body2) {
    CollisionData result;
    result.body1 = body1;
    result.body2 = body2;

    if (body1->shape->type() == BOX && body2->shape->type() == PLANE) {
        return BoxAndPlane(body1, body2);
    }

    if (body1->shape->type() == PLANE && body2->shape->type() == BOX) {
        return BoxAndPlane(body2, body1);
    }

    if (body1->shape->type() == BOX && body2->shape->type() == BOX) {
        return BoxAndBox(body1, body2);
    }

    return result;
}

CollisionData NarrowPhase::BoxAndPlane(RigidBody *boxBody, RigidBody *planeBody) {
    CollisionData collisionData;
    collisionData.body1 = boxBody;
    collisionData.body2 = planeBody;

    // Get the box shape and its vertices
    auto *boxShape = dynamic_cast<BoxShape *>(boxBody->shape.get());
    auto vertices = boxShape->getVerticesWorld(*boxBody);

    // Get the plane shape
    auto *planeShape = dynamic_cast<PlaneShape *>(planeBody->shape.get());
    Vector planeNormal = planeShape->getNormal();

    for (const auto &Q: vertices) {
        Vector boxToPlane = Q - planeShape->getPoint(); // Q - P
        float distance = boxToPlane.dot(planeNormal); // t = (Q - P) * n
        if (distance < 0) {
            // Collision detected
            // The normal should point from the plane towards the box, so it's just the plane's normal.
            collisionData.addContact(Q - planeNormal * distance, planeNormal, -distance);
        }
    }
    return collisionData;
}

CollisionData NarrowPhase::BoxAndBox(RigidBody *boxBody1, RigidBody *boxBody2) {
    CollisionData collisionData;
    collisionData.body1 = boxBody1;
    collisionData.body2 = boxBody2;

    TestVerticesAgainstBox(boxBody1, boxBody2, collisionData, false);
    TestVerticesAgainstBox(boxBody2, boxBody1, collisionData, true);
    return collisionData;
}

void NarrowPhase::TestVerticesAgainstBox(RigidBody *testBody, RigidBody *targetBoxBody, CollisionData &collisionData,
                                         bool flipNormal) {
    auto *testBoxShape = dynamic_cast<BoxShape *>(testBody->shape.get());
    auto *targetBoxShape = dynamic_cast<BoxShape *>(targetBoxBody->shape.get());

    auto vertices = testBoxShape->getVerticesWorld(*testBody);

    // Récupération des axes locaux de targetBox (glm car Quaternion utilise glm)
    glm::vec3 glmAxisX(1.0f, 0.0f, 0.0f);
    glm::vec3 glmAxisY(0.0f, 1.0f, 0.0f);
    glm::vec3 glmAxisZ(0.0f, 0.0f, 1.0f);
    glm::vec3 rawX = targetBoxBody->orientation.rotateVector(glmAxisX);
    glm::vec3 rawY = targetBoxBody->orientation.rotateVector(glmAxisY);
    glm::vec3 rawZ = targetBoxBody->orientation.rotateVector(glmAxisZ);
    Vector axisX(rawX.x, rawX.y, rawX.z);
    Vector axisY(rawY.x, rawY.y, rawY.z);
    Vector axisZ(rawZ.x, rawZ.y, rawZ.z);
    Vector axes[3] = {axisX, axisY, axisZ};

    // Demi-dimensions de targetBox
    const Vector &targetDims = targetBoxShape->getDimensions();
    float halfDims[3] = {
        targetDims.x / 2.0f,
        targetDims.y / 2.0f,
        targetDims.z / 2.0f
    };

    for (const auto &Q: vertices) {
        Vector relPos = Q - targetBoxBody->center;
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
            Vector finalNormal = flipNormal ? bestNormal * -1.0f : bestNormal;
            collisionData.addContact(Q, finalNormal, minPenetration);
        }
    }
}
