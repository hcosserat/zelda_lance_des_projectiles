#include "Cylinder.h"

// Constructeur
Cylinder::Cylinder(const Vector& center, const Vector& massCenter, const Vector& vel, const Vector& acc,
    const Quaternion& orientation, const Vector& angularVel, const Vector& angularAcc, float mass,
    const Matrix3& invInertiaTensor, float radius, float height)
    : RigidBody(center, massCenter, vel, acc, orientation, angularVel, angularAcc, mass, invInertiaTensor)
    , radius(radius)
    , height(height) {
}

void Cylinder::drawShape() const {
    ofNoFill(); // Dessiner les arêtes uniquement
    ofSetColor(50, 200, 50);
    ofDrawCylinder(0, 0, 0,
        radius, height);
    ofFill();
}
