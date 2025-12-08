#include "Box.h"

// Constructeur
Box::Box(const Vector &center, const Vector &massCenter, const Vector &vel, const Vector &acc,
         const Quaternion &orientation, const Vector &angularVel, const Vector &angularAcc, float mass,
         const Matrix3 &invInertiaTensor, const Vector &dimensions)
    : RigidBody(center, massCenter, vel, acc, orientation, angularVel, angularAcc, mass, invInertiaTensor, BOX)
      , dimensions(dimensions) {
}

void Box::drawShape() const {
    ofNoFill(); // Dessiner les arêtes uniquement
    ofSetColor(200, 50, 50); // Couleur pour la boîte
    ofDrawBox(0, 0, 0,
              dimensions.x, dimensions.y, dimensions.z);
    ofFill();
}
