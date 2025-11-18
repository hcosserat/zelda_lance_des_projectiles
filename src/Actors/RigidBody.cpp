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
                                                        , invInertiaTensorBody(invInertiaTensor) {
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

Matrix3 RigidBody::buildRotationMatrixFromEulerXYZ(const float pitch, const float yaw, const float roll) {
    const float cp = std::cos(pitch), sp = std::sin(pitch);
    const float cy = std::cos(yaw), sy = std::sin(yaw);
    const float cr = std::cos(roll), sr = std::sin(roll);

    const Matrix3 Rx(Vector{1, 0, 0}, Vector{0, cp, sp}, Vector{0, -sp, cp});

    const Matrix3 Ry(Vector{cy, 0, -sy}, Vector{0, 1, 0}, Vector{sy, 0, cy});

    const Matrix3 Rz(Vector{cr, sr, 0}, Vector{-sr, cr, 0}, Vector{0, 0, 1});

    return Rz * Ry * Rx;
}

void RigidBody::updateInvInertiaTensor() {
    const Matrix3 R = buildRotationMatrixFromEulerXYZ(orientation.x, orientation.y, orientation.z);
    const Matrix3 Rt = R.transpose();
    invInertiaTensor = R * invInertiaTensorBody * Rt;
}
