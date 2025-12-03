#include "Axe.h"

// Constructeur
Axe::Axe(const Vector& center, const Vector& massCenter, const Vector& vel, const Vector& acc,
    const Quaternion& orientation, const Vector& angularVel, const Vector& angularAcc, float mass,
    const Matrix3& invInertiaTensor, const Vector& handleDimensions, const Vector& headDimensions)
    : RigidBody(center, massCenter, vel, acc, orientation, angularVel, angularAcc, mass, invInertiaTensor)
    , handleDimensions(handleDimensions)
    , headDimensions(headDimensions) {
}

void Axe::drawShape() const {
    ofNoFill(); // Dessiner les arêtes uniquement
    ofSetColor(50, 50, 200);

    // Dessin du manche (Handle)
    // Le manche est centré sur le centre de masse du RigidBody
    ofDrawBox(0, 0, 0,
        handleDimensions.x, handleDimensions.y, handleDimensions.z);

    // Décaler et dessiner la tête (Head)
    // Nous supposons que le manche et la tête sont alignés sur l'axe Y
    float headYOffset = (handleDimensions.y / 2.0f) + (headDimensions.y / 2.0f);
    ofPushMatrix();
    ofTranslate(0, headYOffset, 0);
    ofDrawBox(0, 0, 0,
        headDimensions.x, headDimensions.y, headDimensions.z);
    ofPopMatrix();

    ofFill();
}