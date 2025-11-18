#pragma once

#include <vector>

#include "../Maths/Vector.h"
#include "../Maths/Matrix3.h"
#include "../Forces/Force.h"
#include "../Maths/Quaternion.h"

enum ShapeType {
    BOX,
    CYLINDER,
    AXE
};

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

    ShapeType shape;
    Vector boxDimensions; // used if shape == BOX
    float radius{}; // used if shape == CYLINDER
    float height{}; // used if shape == CYLINDER
    Vector axeHandleDimensions; // used if shape == AXE
    Vector axeHeadDimensions; // used if shape == AXE

    RigidBody(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
              const Quaternion &orientation, const Vector &angularVel, const Vector &angularAcc, float mass,
              const Matrix3 &invInertiaTensor);

    void integratePos(float dt);

    void integrateOrientation(float dt);

    void addForce(const Force &force);

    void clearAccumulator();

    void updateAccelerationsWithAccumulator();

    void updateInvInertiaTensor();

private:
    //static Matrix3 buildRotationMatrixFromEulerXYZ(float pitch, float yaw, float roll);
};
