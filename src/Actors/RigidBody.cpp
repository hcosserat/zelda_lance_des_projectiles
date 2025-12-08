#include "RigidBody.h"

RigidBody::RigidBody(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
                     const Quaternion &orientation, const Vector &angularVel, const Vector &angularAcc,
                     const float mass,
                     const Matrix3 &invInertiaTensor, ShapeType shape) : center(center)
                                                        , massCenter(massCenter)
                                                        , vel(vel)
                                                        , acc(acc)
                                                        , orientation(orientation)
                                                        , angularVel(angularVel)
                                                        , angularAcc(angularAcc)
                                                        , invMass(mass != 0.f ? 1.f / mass : 0.f)
                                                        , invInertiaTensorBody(invInertiaTensor)
                                                        , invInertiaTensor(invInertiaTensor)
                                                        , shape(shape) {
}

void RigidBody::integratePos(const float dt) {
    if (invMass == 0.0f) {
        // Masse infinie, pas de mouvement
        return;
    }

    // Mise à jour de la vélocité
    vel += acc * dt;

    // Mise à jour de la position (both center and massCenter translate together)
    center += vel * dt;
    massCenter += vel * dt;
}

void RigidBody::integrateOrientation(const float dt) {
    if (invMass == 0.0f) {
        // Masse infinie, pas de mouvement
        return;
    }

    // Mise à jour de la vitesse angulaire
    angularVel += angularAcc * dt;

    // Mise à jour de l'orientation
    const Quaternion omega(0, angularVel.x, angularVel.y, angularVel.z);
    Quaternion deltaOrientation = omega * orientation * 0.5f * dt;
    orientation = orientation + deltaOrientation;
    orientation.normalize();
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

    if (invMass == 0.0f) {
        return;
    }

    for (const auto &f: accumForces) {
        acc += f.force * invMass;

        const Vector r = f.applicationPoint - massCenter;
        const Vector tau = r.cross(f.force);

        angularAcc += invInertiaTensor * tau;
    }
}

void RigidBody::updateInvInertiaTensor() {
    const Matrix3 R = orientation.toRotationMatrix3();
    const Matrix3 Rt = R.transpose();
    invInertiaTensor = R * invInertiaTensorBody * Rt;
}
