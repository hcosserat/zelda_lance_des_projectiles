#pragma once

#include <vector>
#include <memory>

#include "../Maths/Vector.h"
#include "../Maths/Matrix3.h"
#include "../Forces/Force.h"
#include "../Maths/Quaternion.h"
#include "ShapeComponent.h"

class RigidBody {
public:
    Vector center; // centre géométrique
    Vector massCenter; // centre de masse

    Vector vel; // vélocité
    Vector acc; // accéleration

    Quaternion orientation; // orientation (angles d'Euler)
    Vector angularVel; // vitesse angulaire
    Vector angularAcc; // accéleration angulaire

    float invMass; // inverse de la masse totale

    Matrix3 invInertiaTensorBody; // J^{-1} dans le repère de l'objet (constant)
    Matrix3 invInertiaTensor; // J^{-1} dans le repère du monde (mis à jour à chaque frame)

    std::vector<Force> accumForces;

    std::unique_ptr<ShapeComponent> shape; // Owns the shape component

    // Getter for bounding radius - delegates to shape component
    float boundingRadius() const { return shape->boundingRadius(); }

    RigidBody(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
              const Quaternion &orientation, const Vector &angularVel, const Vector &angularAcc, float mass,
              const Matrix3 &invInertiaTensor, std::unique_ptr<ShapeComponent> shapeComponent);

    void integratePos(float dt);

    void integrateOrientation(float dt);

    void addForce(const Force &force);

    void clearAccumulator();

    void updateAccelerationsWithAccumulator();

    void updateInvInertiaTensor();
};
