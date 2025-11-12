#include "RigidBody.h"

RigidBody::RigidBody(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
                     const Vector &orientation, const Vector &angularVel, const Vector &angularAcc, const float mass,
                     const Matrix3 &invInertiaTensor) : center(center)
                                                        , massCenter(massCenter)
                                                        , vel(vel)
                                                        , acc(acc)
                                                        , orientation(orientation)
                                                        , angularVel(angularVel)
                                                        , angularAcc(angularAcc)
                                                        , invMass(mass != 0.f ? 1.f / mass : 0.f)
                                                        , invInertiaTensor(invInertiaTensor) {
}

void RigidBody::integratePos(const float dt) {
    if (invMass == 0.0f) {
        // Masse infinie, pas de mouvement
        return;
    }

    // Mise à jour de la vélocité
    vel += acc * dt;

    // Mise à jour de la position
    center += vel * dt;
}

void RigidBody::integrateOrientation(const float dt) {
    if (invMass == 0.0f) {
        // Masse infinie, pas de mouvement
        return;
    }

    // Mise à jour de la vitesse angulaire
    angularVel += angularAcc * dt;

    // Mise à jour de l'orientation
    orientation += angularVel * dt;
}

void RigidBody::addForce(const Force &force) {
    accumForces.push_back(force);
}

void RigidBody::clearAccumulator() {
    accumForces.clear();
}

void RigidBody::updateAccelerationsWithAccumulator() {
    acc = Vector(0, 0, 0);
    angularAcc = Vector(0, 0, 0);

    for (const auto [force, applicationPoint]: accumForces) {
        acc += force; // todo: on est sûr de tout prendre ?

        Vector l = massCenter - applicationPoint;
        Vector tau = force.cross(l);
        angularAcc += invInertiaTensor * tau; // todo: normalement c'est tau en ligne * matrice mais jsp
    }
}

